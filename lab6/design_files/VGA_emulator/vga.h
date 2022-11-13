#ifndef _VGA_H_
#define _VGA_H_

#include <string>

// Display constants
#define VGA_DISPLAY_RES_X   640
#define VGA_DISPLAY_RES_Y   480

// Display
void draw_pixel(unsigned x, unsigned y, unsigned short color);
unsigned short read_pixel(volatile void *pixel_buf, int x, int y);
void init_phy_mem_mapping_read_vga_ctrl();
void* resolve_current_pixel_buffers();
void init_display();
void init_window(int x, int y);
void init_gc(int reverse_video);
void init_colormap();
void close_window();
void main_loop();

// Device
int open_physical(int fd);
void close_physical(int fd);
void * map_physical(int fd, unsigned int base, unsigned int span);
int unmap_physical(void *virtual_base, unsigned int span);

// MISC
void init_ascii_map(char * exe_path);
char * str_trim(char *str);
std::string get_exe_base_dir(char * exe_path);

#endif
