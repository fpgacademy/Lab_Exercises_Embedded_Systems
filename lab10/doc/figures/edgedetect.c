#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <intelfpgaup/video.h>

typedef unsigned char byte;
struct pixel { // Pixel data structure. Ordering of pixel colors in BMP files is b, g, r
   byte b;
   byte g;
   byte r;
};

// Read BMP file and extract the header (store in header) and pixel values (store in data)
int read_bmp(char* filename, byte** header, struct pixel** data, int *width, int *height) {
   struct pixel * data_;	// temporary pointer to pixel data
   byte * header_;			// temporary pointer to header data
	int width_, height_;		// temporary variables for width and height

   FILE* file = fopen (filename, "rb");
   if (!file) return -1;
   
   // read the 54-byte header
   header_ = malloc (54);
   fread (header_, sizeof(byte), 54, file); 

   // get height and width of image
   width_ = *(int*) &header_[18];	// width is given by four bytes starting at offset 18
   height_ = *(int*) &header_[22];	// height is given by four bytes starting at offset 22

   // Read in the image
   int size = width_ * height_;
   data_= malloc (size * sizeof(struct pixel)); 
   fread (data_, sizeof(struct pixel), size, file); // read the rest of the data
   fclose (file);
   
   *header = header_;	// return pointer to caller
   *data = data_;			// return pointer to caller
	*width = width_;		// return value to caller
	*height = height_;	// return value to caller
   
   return 0;
}

// Determine the grayscale 8 bit value by averaging the r, g, and b channel values.
// Store the 8 bit grayscale value in the r channel.
void convert_to_grayscale(struct pixel *data, int width, int height) {
   int x, y;
   
   for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
      	// Just use the 8 bits in the r field to hold the entire grayscale image
         (*(data + y*width + x)).r = ((*(data + y*width + x)).r + (*(data + y*width + x)).g +
            (*(data + y*width + x)).b)/3;
      }
   }
}

// Write the grayscale image to disk. The 8-bit grayscale values should be inside the
// r channel of each pixel.
void write_grayscale_bmp(char *bmp, byte *header, struct pixel *data, int width, int height) {
   FILE* file = fopen (bmp, "wb");
   
   // write the 54-byte header
   fwrite (header, sizeof(byte), 54, file); 
   int y, x;
   
   // the r field of the pixel has the grayscale value. Copy to g and b.
   for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
         (*(data + y*width + x)).b = (*(data + y*width + x)).r;
         (*(data + y*width + x)).g = (*(data + y*width + x)).r;
      }
   }
   int size = width * height;
   fwrite (data, sizeof(struct pixel), size, file); // write the rest of the data
   fclose (file);
}

// Draw the image pixels on the VGA display
void draw_image (struct pixel *data, int width, int height, int screen_x, int screen_y) {
	// ... code not shown
}

// Gaussian blur. Operate on the .r fields of the pixels only.
void gaussian_blur( ) {
   unsigned int gaussian_filter[5][5] = {
      { 2, 4, 5, 4, 2 },
      { 4, 9,12, 9, 4 },
      { 5,12,15,12, 5 },
      { 4, 9,12, 9, 4 },
      { 2, 4, 5, 4, 2 }
   };
	// ... code not shown
}

void sobel_filter( ) {
   // Definition of Sobel filter in horizontal and veritcal directions
   int horizontal_operator[3][3] = {
      { -1,  0,  1 },
      { -2,  0,  2 },
      { -1,  0,  1 }
   };
   int vertical_operator[3][3] = {
      { -1,  -2,  -1 },
      { 0,  0,  0 },
      { 1,  2,  1 }
   };
	// ... code not shown
}

void non_maximum_suppressor( ) {
	// ... code not shown
}

// Only keep pixels that are next to at least one strong pixel.
void hysteresis_filter( ) {
   #define strong_pixel_threshold 32	// example value
	// ... code not shown
}

int main(int argc, char *argv[]) {
   struct pixel *data;								// used to hold the image pixels
   byte *header;										// used to hold the image header
	int width, height;								// the dimensions of the image
	int screen_x, screen_y, char_x, char_y;	// VGA screen dimensions
   time_t start, end;								// used to measure the program's run-time
   
   // Check inputs
   if (argc < 2) {
      printf ("Usage: edgedetect <BMP filename>\n");
      return 0;
   }

   // Open input image file (24-bit bitmap image)
   if (read_bmp (argv[1], &header, &data, &width, &height) < 0) {
      printf ("Failed to read BMP\n");
      return 0;
   }
   
   if (!video_open ())	// open the VGA display driver
   {
      printf ("Error: could not open video device\n");
      return -1;
   }
   video_read (&screen_x, &screen_y, &char_x, &char_y);   // get VGA screen size
   draw_image (data, width, height, screen_x, screen_y);

   /********************************************
   *          IMAGE PROCESSING STAGES          *
   ********************************************/
   
   // Start measuring time
   start = clock();
   
   convert_to_grayscale(data, width, height);
   // gaussian_blur (...);
   // sobel_filter (...);
   // non_maximum_suppressor (...);
   // hysteresis_filter (...);
   
   end = clock();
   
   printf ("TIME ELAPSED: %.0f ms\n", ((double) (end - start)) * 1000 / CLOCKS_PER_SEC);
   
   write_grayscale_bmp ("edges.bmp", header, data, width, height);
   
   printf ("Press return to continue");
   getchar ();
   draw_image (data, width, height, screen_x, screen_y);
   video_close ( );
   return 0;
}
