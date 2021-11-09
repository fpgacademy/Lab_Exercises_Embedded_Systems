#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include "physical.h"

typedef unsigned char byte;
int width, height;

// Declared data structure for a pixel. The ordering of pixel colors in BMP files is b, g, r
struct pixel {
    byte b;
    byte g;
    byte r;
};

// Read BMP file and extract the pixel values (store in data) and header (store in header)
// Data is data[0] = BLUE, data[1] = GREEN, data[2] = RED, data[3] = BLUE, etc...
int read_bmp(char *filename, unsigned char **header, struct pixel **data) {
     struct pixel *data_tmp;
     unsigned char *header_tmp;
     FILE *file = fopen (filename, "rb");
     
     if (!file) return -1;
     
     // read the 54-byte header
     header_tmp = malloc (54 * sizeof(unsigned char));
     fread (header_tmp, sizeof(unsigned char), 54, file); 

     // get height and width of image from the header
     width = *(int*)(header_tmp + 18);  // width is a 32-bit int at offset 18
     height = *(int*)(header_tmp + 22); // height is a 32-bit int at offset 22

     // Read in the image
     int size = width * height;
     data_tmp = malloc (size * sizeof(struct pixel)); 
     fread (data_tmp, sizeof(struct pixel), size, file); // read the data
     fclose (file);
     
     *header = header_tmp;
     *data = data_tmp;
     
     return 0;
}

// This function flips the image vertically. It first swaps the pixel values in the top and 
// bottom rows, then the second row with the second-from-bottom, and so on, until finally 
// swapping the middle two rows.
void flip (struct pixel *data, int width, int height){
     int i, j;
     struct pixel tmp;
     // declare image as a 2-D array so that we can use the syntax image[row][column]
     struct pixel (*image)[width] = (struct pixel (*)[width]) data;
     
     for (i = 0; i < height / 2; ++i)
         for (j = 0; j < width; ++j) {
             // ... code not shown
         }
}

// The video IP cores used for edge detection require the RGB 24 bits of each pixel to be
// word aligned (aka 1 byte of padding per pixel):
// | unused 8 bits  | red 8 bits | green 8 bits | blue 8 bits |
void memcpy_consecutive_to_padded(struct pixel *from, volatile unsigned int *to, int pixels){
    // ... code not shown
}

int main(int argc, char *argv[]){
    struct pixel *data;    // used to hold the image pixels
    byte *header;          // used to hold the image header
    int fd = -1;           // used to open/read/etc. the image file
    void *LW_virtual;
    void *Onchip_virtual;

    // Pointer to the DMA controller for the original image
    volatile unsigned int *mem_to_stream_dma = NULL;
    
    // Check inputs
    if (argc < 2) {
        printf ("Usage: edgedetect <BMP filename>\n");
        return -1;
    }
    // Open input image file (24-bit bitmap image)
    if (read_bmp (argv[1], &header, &data) < 0){
        printf ("Failed to read BMP\n");
        return -1;
    }
    printf ("Image width = %d pixels, Image height = %d pixels\n", width, height);
    if (width != 240 || height != 240) {
        printf ("Width or height not compatible with hardware (must be 240)\n");
        return -1;
    }
    if ((fd = open_physical (fd)) == -1)   // Open /dev/mem
        return (-1);

    LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    Onchip_virtual = map_physical (fd, FPGA_ONCHIP_BASE, FPGA_ONCHIP_SPAN);
    if ((LW_virtual == NULL) || (Onchip_virtual == NULL))
        return (-1);

    // Set up pointer to edge-detection DMA controller using address in the FPGA system
    mem_to_stream_dma = (volatile unsigned int *)(LW_virtual + 0x3100);
    *(mem_to_stream_dma+3) = 0; // Turn off edge-detection hardware DMA
    
    // Flip the image upside down, so it will display correctly on VGA
    flip (data, width, height);

    // Write the image to the memory used for video-out and edge-detection
    memcpy_consecutive_to_padded (data, Onchip_virtual, width*height);

    free(header);
    free(data);
    
    unmap_physical (Onchip_virtual, FPGA_ONCHIP_SPAN);
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);
    close_physical (fd);    // close /dev/mem
    
    return 0;
}

