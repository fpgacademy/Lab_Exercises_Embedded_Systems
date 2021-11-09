#define SW_BYTES			1		// number of characters to read from /dev/SW
#define KEY_BYTES			1		// number of characters to read from /dev/KEY

#define KEY0 				'1'
#define KEY1 				'2'
#define KEY2				'4'
#define KEY3				'8'

#define FASTER				1
#define SLOWER				2

#define ABS(x)				(((x) > 0) ? (x) : -(x))

/* Constants for animation */
#define SCREEN_X			80
#define SCREEN_Y			24
#define NUM_XS				5		// default number of Xs to draw
#define NUM_XS_MAX		25		// maximum number of Xs to draw
#define FALSE				0
#define TRUE				1
#define ANIMATION_T 		200000000		// default nano secds (0.2 seconds)

#define RED 				31
#define GREEN 				32
#define YELLOW 			33
#define BLUE 				34
#define MAGENTA 			35
#define CYAN 				36
#define WHITE 				37
