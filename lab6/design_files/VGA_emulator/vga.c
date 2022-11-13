#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <vector>
#include <sstream>

#include "vga.h"
#include "address_map_arm.h"

// Global Variables //
struct x11_instance_s {
    // X11
    Display *display;
    int screen_num;
    int win_border_width;
    Window win;
    char *display_name;
    GC gc;
    Colormap screen_colormap;
    XColor color_16bit[65536];
    unsigned short color_8_to_16_map[256]; 
    Status rc;

    // ASCII map for 8x8 char map lookup.
    char ascii_map[256][8][8];
    
    // Misc
    unsigned long curr_forground_pixel;
} x11;

// FPGA
struct board_emulator_mem_s {
    void *lwbridge;
    void *chara;
} emulator_mem;

// FPGA - Pixel
struct pixel_ctrl_s {
    volatile unsigned int *base_ptr;
    unsigned int phy_buf_ptr[2];
    void *vir_buf_ptr[2];
    unsigned int resolution[2];

    unsigned int status;
    // Derivatives of the status bit.
    unsigned int sizeof_pixel;      // B
    unsigned int video_m, video_n;  // m, n
    unsigned int address_mode;      // A
} pixel_ctrl;

// FPGA - Char
struct char_ctrl_s {
    volatile unsigned int *base_ptr;
} char_ctrl;
//volatile unsigned int *char_ctrl;

// Used for /dev/mem
int fd = -1;
//////////////////////

int main(int argc, char* argv[]) {
    // Display ENV variable
    x11.display_name = getenv("DISPLAY");
    x11.win_border_width = 2;

    printf("%s\n", argv[0]);

    // Read MIF file and initialize char map.
    init_ascii_map(argv[0]);

    // Initialize physical memory mapping and reading of VGA control registers.
    init_phy_mem_mapping_read_vga_ctrl();

    // Initialize the display.
    init_display();

    /* create a simple window, as a direct child of the screen's   */
    /* root window. Use the screen's white color as the background */
    /* color of the window. Place the new window's top-left corner */
    /* at the given 'x,y' coordinates.                             */
    init_window(0, 0);

    // Allocate and initialize the GC (graphics context) for drawing in the window.
    init_gc(0);
    XSync(x11.display, False);

    // Initialize the color map.
    init_colormap();
    
    // Default foreground to black.
    XSetForeground(x11.display, x11.gc, x11.color_16bit[0].pixel);
    x11.curr_forground_pixel = 0;

    // Run the main loop.
    main_loop();

}

// TODO: Handle different address formats (consecutive or matrix style - default).
// TODO: Handle other status register bits?

void main_loop() {
    printf("Running Emulator\n");

    // A few local variables.
    volatile void *vga_buf;
    volatile char *char_buf;

    unsigned short tmp_pixel, tmp_char_pixel;
    unsigned char tmp_char;
    int pixel_offset, char_offset;
    int x, y, char_x, char_y, pixel_x, pixel_y;
    int tmp_x, tmp_y;

    const unsigned short white_pixel = (pixel_ctrl.sizeof_pixel == 1) ? 0xFF : 0xFFFF;

    // This is the main loop.
    while(1) {
        // Resolve the buffer memory and get the current virtual pointer.
        vga_buf = resolve_current_pixel_buffers();

        if(vga_buf == NULL) {
            fprintf(stderr, "Error in resolving the pixel buffer address.");
			// EXIT POINT: unmap the region, close the physical device, close the window.
			// TODO: Catch sigint?
			unmap_physical(emulator_mem.lwbridge, LW_BRIDGE_SPAN);
			unmap_physical(emulator_mem.chara, FPGA_CHAR_SPAN);
			close_physical(fd);
			close_window();
        }

		// TODO: Is there a back buffer for characters?
		char_buf = (char *) emulator_mem.chara;

		// Draw pixels (and characters) of the VGA.
		for(y = 0; y < VGA_DISPLAY_RES_Y; y += 8) {
			for(x = 0; x < VGA_DISPLAY_RES_X; x += 8) {
				tmp_x = x/8;
				tmp_y = y/8;

				// Getting character if enabled
				char_offset = (tmp_y << 7) | tmp_x;
				tmp_char = *(char_buf + char_offset);

				// Draw a single character (and pixels in it), 8x8.
				for(char_y = 0; char_y < 8; char_y += 2) {
				    for(char_x = 0; char_x < 8; char_x += 2) {
					    // Pixel X and Y positions in memory.
					    tmp_x = (x + char_x)/2;
					    tmp_y = (y + char_y)/2;

                        // Read the pixel.
                        tmp_pixel = read_pixel(vga_buf, tmp_x, tmp_y);

					    // Drawing a single pixel, 2x2.
					    for(pixel_y = 0; pixel_y < 2; pixel_y++) {
					        for(pixel_x = 0; pixel_x < 2; pixel_x++) {
						        // Get value of current character.
						        tmp_char_pixel = (unsigned short) x11.ascii_map[tmp_char][char_y + pixel_y][char_x + pixel_x];

						        // ON = white color.
						        if(tmp_char_pixel != 0) {
						            tmp_char_pixel = white_pixel;
						        }

						        // Draw the pixel.
                                draw_pixel(x + char_x + pixel_x, y + char_y + pixel_y, tmp_pixel|tmp_char_pixel);
					        }
					    }  // END: 2x2 double for loop for pixel drawing.
				    }
				}  // END: 8x8 double for loop for character drawing
			}  // END: Column-wise drawing (inner) for loop.
		}  // END: Row-wise drawing (outer) for loop

		// Flush requests to X server.
		XFlush(x11.display);
    }  // END: Main while loop.
}

void draw_pixel(unsigned x, unsigned y, unsigned short color) {
    // Adjust color if we are using 8 bit.
    if(pixel_ctrl.sizeof_pixel == 1) {
        color = x11.color_8_to_16_map[color];
    }
    
    // Set foreground if needed.
    if(x11.curr_forground_pixel != x11.color_16bit[color].pixel) {
        XSetForeground(x11.display, x11.gc, x11.color_16bit[color].pixel);
        x11.curr_forground_pixel = x11.color_16bit[color].pixel;
    }
    
    // Draw point
	XDrawPoint(x11.display, x11.win, x11.gc, x, y);
}

/**
 * Helper function to read the pixel at (x,y) from the current pixel buffer memory.
 * @param pixel_buf - The (void) pointer to the pixel buffer to read from.
 * #param x - X position to read.
 * @param y - Y position to read.
 * @return the pixel at this position.
 */
unsigned short read_pixel(volatile void *pixel_buf, int x, int y) {
    // Find the  offset to pixel (based on address mode bit - A).
    int pixel_offset;
    if(pixel_ctrl.address_mode == 0) {
        // X, Y address mode.
        const int shift_x = pixel_ctrl.sizeof_pixel - 1; // shift x address bits by sizeof(pixel)
        const int shift_y = pixel_ctrl.video_n + (pixel_ctrl.sizeof_pixel - 1);// shift y address by |x address| + sizeof(pixel)
        pixel_offset = (y << shift_y) + (x << shift_x);
    } else {
        // Consecutive address mode.
        pixel_offset = (y * pixel_ctrl.video_n * pixel_ctrl.sizeof_pixel) + (x * pixel_ctrl.sizeof_pixel);
    }

    // Return based on the pixel size (1 vs 2 bytes).
    if(pixel_ctrl.sizeof_pixel == 1) {
        return (short)(*(unsigned char *)(pixel_buf + pixel_offset));
    } else {
        return *(unsigned short *)(pixel_buf + pixel_offset);
    }
}

void* resolve_current_pixel_buffers() {
    // Get the current front and back buffers.
    unsigned int tmp_phy_buf[2];
    tmp_phy_buf[0] = (unsigned long)(*pixel_ctrl.base_ptr);
    tmp_phy_buf[1] = (unsigned long)(*(pixel_ctrl.base_ptr + 1));

    if(pixel_ctrl.vir_buf_ptr[0] == NULL || pixel_ctrl.vir_buf_ptr[0] == NULL) {
        ///// Initialize case /////
        // Initialize front buffer.
        if((pixel_ctrl.vir_buf_ptr[0] = map_physical(fd, tmp_phy_buf[0], FPGA_ONCHIP_SPAN)) == NULL) {
    		exit(-1);
        }

        // Initialize back buffer (if it exists).
        if(tmp_phy_buf[0] != tmp_phy_buf[1]) {
            // 2 buffers.
    		if((pixel_ctrl.vir_buf_ptr[1] = map_physical(fd, tmp_phy_buf[1], FPGA_ONCHIP_SPAN)) == NULL) {
    			exit(-1);
    		}
        } else {
            // Only 1 buffer.
    		pixel_ctrl.vir_buf_ptr[1] = pixel_ctrl.vir_buf_ptr[0];
        }
    } else {
        ///// Not initialize case /////
        const int phy_buffers_same = (tmp_phy_buf[0] == pixel_ctrl.phy_buf_ptr[0] && tmp_phy_buf[1] == pixel_ctrl.phy_buf_ptr[1]);
        const int phy_buffers_swap = (tmp_phy_buf[0] == pixel_ctrl.phy_buf_ptr[1] && tmp_phy_buf[1] == pixel_ctrl.phy_buf_ptr[0]);

        if(!phy_buffers_same) {
            if(phy_buffers_swap) {
                ///// Buffers swapped case /////
                // Buffers are swapped, so swap our virtual pointers (avoid re-map)
                void * tmp = pixel_ctrl.vir_buf_ptr[0];
                pixel_ctrl.vir_buf_ptr[0] = pixel_ctrl.vir_buf_ptr[1];
                pixel_ctrl.vir_buf_ptr[1] = tmp;
            } else {
                ///// Buffers changed, but not swapped. /////
                // Some buffers have been changed dynamically.
                // Unmap the current ones.
                unmap_physical(pixel_ctrl.vir_buf_ptr[0], FPGA_ONCHIP_SPAN);
                if(pixel_ctrl.vir_buf_ptr[0] != pixel_ctrl.vir_buf_ptr[1]) {
                    unmap_physical(pixel_ctrl.vir_buf_ptr[1], FPGA_ONCHIP_SPAN);
                }

                // Map the new ones.
                if((pixel_ctrl.vir_buf_ptr[0] = map_physical(fd, tmp_phy_buf[0], FPGA_ONCHIP_SPAN)) == NULL) {
                    exit(-1);
                }

                if(tmp_phy_buf[0] != tmp_phy_buf[1]) {
                    if((pixel_ctrl.vir_buf_ptr[1] = map_physical(fd, tmp_phy_buf[1], FPGA_ONCHIP_SPAN)) == NULL) {
                        exit(-1);
                    }
                } else {
                    pixel_ctrl.vir_buf_ptr[1] = pixel_ctrl.vir_buf_ptr[0];
                }
            }
        } ///// ELSE: Nothing changed from last cycle, do nothing. /////
    }

    // Store physical buffer pointers.
    pixel_ctrl.phy_buf_ptr[0] = tmp_phy_buf[0];
    pixel_ctrl.phy_buf_ptr[1] = tmp_phy_buf[1];

    // Return current front buffer (virtual) addres.
    return pixel_ctrl.vir_buf_ptr[0];
}

void init_phy_mem_mapping_read_vga_ctrl() {
    // Setting up virtual to physical mappings.
    // Open /dev/mem
    if((fd = open_physical(fd)) == -1) {
		exit(-1);
    }

    // Map the lightweight bridge
    if((emulator_mem.lwbridge = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL) {
		exit(-1);
    }

    // Read VGA control registers to initialize VGA variables.
    pixel_ctrl.base_ptr = (unsigned int *)(emulator_mem.lwbridge + PIXEL_BUF_CTRL_BASE);

    // Read the current front and back buffers.
    pixel_ctrl.phy_buf_ptr[0] = (unsigned long)(*pixel_ctrl.base_ptr);
    pixel_ctrl.phy_buf_ptr[1] = (unsigned long)(*(pixel_ctrl.base_ptr + 1));

    // Read the resolution register
    unsigned yx_res = (unsigned)(*(pixel_ctrl.base_ptr + 2));
    pixel_ctrl.resolution[0] = yx_res & 0x0000FFFF;
    pixel_ctrl.resolution[1] = (yx_res >> 16) & 0x0000FFFF;

    // Read the status register and pick out the derivatives of the status (A, B, m, n bits)
    pixel_ctrl.status = (unsigned)(*(pixel_ctrl.base_ptr + 3));
    pixel_ctrl.sizeof_pixel = ((pixel_ctrl.status & 0xF00) == 0x700) ? 1 : 2;	// check bits 11-8 for sizeof (pixel)
    pixel_ctrl.video_m = (pixel_ctrl.status & 0xFF000000) >> 24; // width of y address
    pixel_ctrl.video_n = (pixel_ctrl.status & 0x00FF0000) >> 16; // width of x address
    pixel_ctrl.address_mode = (pixel_ctrl.status & 0x2) >> 1;   // Address mode (A bit)

    // DEBUG PRINT
    printf("Front buffer address: 0x%x\nBack buffer address: 0x%x\nPixel Resolution: (%u, %u)\nStatus: 0x%x\n",
		pixel_ctrl.phy_buf_ptr[0], pixel_ctrl.phy_buf_ptr[1], pixel_ctrl.resolution[0], pixel_ctrl.resolution[1], pixel_ctrl.status);
    printf("Size of pixel: %d\nAddress mode bit: %d\n\n", pixel_ctrl.sizeof_pixel, pixel_ctrl.address_mode);

    // Set to NULL to start.
    pixel_ctrl.vir_buf_ptr[0] = NULL;
    pixel_ctrl.vir_buf_ptr[1] = NULL;

    // Map the character buffer memory (fixed).
    if((emulator_mem.chara = map_physical(fd, FPGA_CHAR_BASE, FPGA_CHAR_SPAN)) == NULL) {
		exit(-1);
    }
}

void init_display() {
    // open connection with the X server.
    x11.display = XOpenDisplay(x11.display_name);
    if (x11.display == NULL) {
		fprintf(stderr, "Cannot connect to X server '%s'.\n", x11.display_name);
		exit(1);
    }

    // get the geometry of the default screen for our display.
    x11.screen_num = DefaultScreen(x11.display);
    printf("Display dimensions: (%d, %d)\n", pixel_ctrl.resolution[0], pixel_ctrl.resolution[1]);
}

/*
 * function: create_simple_window. Creates a window with a white background
 *           in the given size.
 * input:    display, size of the window (in pixels), and location of the window
 *           (in pixels).
 * output:   the window's ID.
 * notes:    window is created with a black border, 2 pixels wide.
 *           the window is automatically mapped after its creation.
 */
void init_window(int x, int y) {
    // Set screen number
    x11.screen_num = DefaultScreen(x11.display);

    // Create a simple window, as a direct child of the screens
    // root window. Use the screen's black and white colors as
    // the foreground and background colours of the window.
    // Place the new winodws top left corner at the given coordinates.
    x11.win = XCreateSimpleWindow(x11.display, RootWindow(x11.display, x11.screen_num),
                            x, y, VGA_DISPLAY_RES_X, VGA_DISPLAY_RES_Y, x11.win_border_width,
                            BlackPixel(x11.display, x11.screen_num),
                            WhitePixel(x11.display, x11.screen_num));

    // What inputs register events.
    //XSelectInput(display, win, ExposureMask|ButtonPressMask|KeyPressMask);

    // Make the window actually appear on the screen.
    XMapWindow(x11.display, x11.win);

    // Flush all pending requests to the X server.
    XFlush(x11.display);
}

void init_gc(int reverse_video) {
    unsigned long valuemask = 0;	/* which values in 'values' to  */
					/* check when creating the GC.  */
    XGCValues values;			/* initial values for the GC.   */
    unsigned int line_width = 2;	/* line width for the GC.       */
    int line_style = LineSolid;		/* style for lines drawing and  */
    int cap_style = CapButt;		/* style of the line's edje and */
    int join_style = JoinBevel;		/*  joined lines.		*/
    int screen_num = DefaultScreen(x11.display);

    x11.gc = XCreateGC(x11.display, x11.win, valuemask, &values);
    if(x11.gc < 0) {
		fprintf(stderr, "XCreateGC: \n");
    }

    // allocate foreground and background colors for this GC.
    if (reverse_video) {
		XSetForeground(x11.display, x11.gc, WhitePixel(x11.display, x11.screen_num));
		XSetBackground(x11.display, x11.gc, BlackPixel(x11.display, x11.screen_num));
    } else {
		XSetForeground(x11.display, x11.gc, BlackPixel(x11.display, x11.screen_num));
		XSetBackground(x11.display, x11.gc, WhitePixel(x11.display, x11.screen_num));
    }

    // define the style of lines that will be drawn using this GC.
    XSetLineAttributes(x11.display, x11.gc, line_width, line_style, cap_style, join_style);

    // define the fill style for the GC. to be 'solid filling'.
    XSetFillStyle(x11.display, x11.gc, FillSolid);
}

// Very specific colours.
// ORDER: yellow, red, green, blue, cyan, magenta, grey, pink, orange
const unsigned short specific_8_bit_colours[9] = {0xffe0, 0xf800, 0x07e0, 0x041f, 0x07ff, 0xf81f, 0xc618, 0xfc18, 0xfc00};
const unsigned short specific_16_bit_colours[9] = {0xfc, 0xe0, 0x1c, 0x03, 0x1f, 0xe3, 0x6d, 0xed, 0xf0};

void init_colormap() {
    printf("Setting up color mapping...\n");
    x11.screen_colormap = DefaultColormap(x11.display, x11.screen_num);

    int r, g, b;
    int i;

    // 16-bit (2 byte) map.
    i = 0;
    for(r = 0; r < 32; r++) {
	    for(g = 0; g < 64; g++) {
	        for(b = 0; b < 32; b++) {
		        x11.color_16bit[i].pixel = i;
		        x11.color_16bit[i].flags = DoRed|DoGreen|DoBlue;
		        x11.color_16bit[i].red = r << 11;
		        x11.color_16bit[i].green = g << 10;
		        x11.color_16bit[i].blue = b << 11;

		        if(XAllocColor(x11.display, x11.screen_colormap, &x11.color_16bit[i]) == 0) {
			        fprintf(stderr, "XAllocColor failed.\n");
		        }

		        i++;
	        }
	    }
    }
    
    // Create 8 to 16 bit conversion mapping.
    for(i = 0; i < 256; i++) {
	const unsigned short r = ((i >> 5) & 0x3) * 4;
	const unsigned short g = ((i >> 2) & 0x3) * 8;
	const unsigned short b = ((i >> 0) & 0x2) * 8;

	x11.color_8_to_16_map[i] = (r << 11) | (g << 5) | (b);
    }

    // Special mappings.
    for(i = 0; i < 9; i++) {
        x11.color_8_to_16_map[specific_8_bit_colours[i]] = specific_16_bit_colours[i];
    }
}

void close_window() {
    XFreeColormap(x11.display, x11.screen_colormap);
    XFreeGC(x11.display, x11.gc);
    XDestroyWindow(x11.display, x11.win);
    XCloseDisplay(x11.display);
    exit(-1);
}

// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical(int fd)
{
    if(fd == -1) {
	if((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
		fprintf(stderr, "ERROR: could not open \"/dev/mem\"...\n");
		return -1;
	}
    }

    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical(int fd) {
    close(fd);
}

/**
 * Establish a virtual address mapping for the physical addresses starting at base, and extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if(virtual_base == MAP_FAILED) {
	fprintf(stderr, "ERROR: mmap() failed...\n");
	close(fd);
	return NULL;
    }

    return virtual_base;
}

/**
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span) {
    if(munmap(virtual_base, span) != 0) {
		fprintf(stderr, "ERROR: munmap() failed...\n");
		return -1;
    }
    return 0;
}

/**
 * Initializes the ASCII mapping from 8 bit value to character bitmap.
 */
void init_ascii_map(char * exe_path) {
    printf("Parsing MIF file for ASCII mapping for characters...\n");

    char buf[256];
    int read_content = 0;
    int letter_index = 0;
    int letter_row_index = 0;
    int ret;
    char *ascii_map_ptr;

    // Reading the mif file.
    std::string mif_path = get_exe_base_dir(exe_path) + "ascii_table.mif";
    FILE *f = fopen(mif_path.c_str(), "r");

    // Could not find initialization file.
    if(f == NULL) {
        fprintf(stderr, "Could not find initialization file: 'ascii_table.mif'.\n"); exit(1);
    }

    while(fgets(buf, sizeof(buf), f)) {
        // Trim leading and trailing space.
        str_trim(buf);

        if(read_content) {
            if(!strcmp(buf, "END;")) {
                read_content = 0;
            } else {
                // Reading content.
                ascii_map_ptr = x11.ascii_map[letter_index][letter_row_index];
                ret = sscanf(buf, "%*d : %d %d %d %d %d %d %d %d;",
                &ascii_map_ptr[0], &ascii_map_ptr[1], &ascii_map_ptr[2], &ascii_map_ptr[3],
                &ascii_map_ptr[4], &ascii_map_ptr[5], &ascii_map_ptr[6], &ascii_map_ptr[7]);

                // Move to next row.
                letter_row_index = (letter_row_index + 1) % 8;

                // If we wrap around to 0, move to next letter.
                if(letter_row_index == 0) {
                    letter_index++;
                }
            }
        } else {
            if(!strcmp(buf, "CONTENT BEGIN")) {
                read_content = 1;
            }
        }
    }

    // Special case for ascii value of 0 (null).
    int i,j;
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
            x11.ascii_map[0][i][j] = 0;


    fclose(f);
}

/**
* Trims the leading and trailing spaces of string (in place).
* @param str - The string to trim the leading and trailing space of (in place).
* @return char * - The pointer to the beginning of the new string.
*/
char * str_trim(char *str) {
    char *end;

    // Trim leading.
    while(isspace((unsigned char) *str)) {
        str++;
    }

    if(*str == 0) {
        return str;
    }

    // Trim trailing
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // New nul terminate.
    *(end + 1) = '\0';

    return str;
}


/**
 * @return std::string - The path to the base directory of the EXE (relative path).
 */
std::string get_exe_base_dir(char * exe_path) {
    std::stringstream tmp(exe_path);
    std::string seg;
    std::vector<std::string> seglist;

    while(std::getline(tmp, seg, '/')) {
        seglist.push_back(seg);
    }

    std::string ret("");
    for(int i = 0; i < seglist.size() - 1; i++) {
        ret += seglist[i];
        ret += "/";
    }

    return ret;
}

