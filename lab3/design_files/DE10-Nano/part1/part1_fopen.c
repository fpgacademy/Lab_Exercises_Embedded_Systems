#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define chardev_BYTES 256				// max number of characters to read from /dev/chardev

volatile sig_atomic_t stop;
void catchSIGINT(int signum){
    stop = 1;
}

/* This code provides an alternative implementation for Part I. It uses the "fopen" family of
 * library files rather than the "open" family. The choice of whether to use "fopen" or "open"
 * functions is a matter of the user's personal preference. 
 *
 * This code uses the character device driver /dev/chardev. The code reads the default
 * message from the driver and then prints it. After this the code changes the message in
 * a loop by writing to the driver, and prints each new message. The program exits if it
 * receives a kill signal (for example, ^C typed on stdin). */
int main(int argc, char *argv[]){

	FILE *chardev_fp;							// file pointer
  	char chardev_buffer[chardev_BYTES];	// buffer for chardev character data
	char new_msg[128];						// space for the new message that we generate
	int i_msg;
    
  	// catch SIGINT from ctrl+c, instead of having it abruptly close this program
  	signal(SIGINT, catchSIGINT);
    
	if ((chardev_fp = fopen("/dev/chardev", "r+")) == NULL)
	{
		printf ("Error opening /dev/chardev: %s\n", strerror(errno));
		return -1;
	}

	i_msg = 0;
  	while (!stop)
	{
  		while (fgets (chardev_buffer, chardev_BYTES, chardev_fp))
			;	// read the driver until EOF

		printf ("%s", chardev_buffer);

		sprintf (new_msg, "New message %d\n", i_msg);
		i_msg++;
		fputs (new_msg, chardev_fp);
		fflush (chardev_fp);

		sleep (1);
	}
	fclose (chardev_fp);
   return 0;
}
