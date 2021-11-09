#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define BYTES 256				// max number of characters to read from /dev/chardev

volatile sig_atomic_t stop;
void catchSIGINT(int signum){
    stop = 1;
}

/* This code uses the character device driver /dev/chardev. The code reads the default
 * message from the driver and then prints it. After this the code changes the message in
 * a loop by writing to the driver, and prints each new message. The program exits if it
 * receives a kill signal (for example, ^C typed on stdin). */
int main(int argc, char *argv[]){

	int chardev_FD;							// file descriptor
  	char chardev_buffer[BYTES];			// buffer for chardev character data
	int ret_val, chars_read;				// number of characters read from chardev
	char new_msg[128];						// space for the new message that we generate
	int i_msg;
    
  	// catch SIGINT from ctrl+c, instead of having it abruptly close this program
  	signal(SIGINT, catchSIGINT);
    
	// Open the character device driver for read/write
	if ((chardev_FD = open("/dev/chardev", O_RDWR)) == -1)
	{
		printf("Error opening /dev/chardev: %s\n", strerror(errno));
		return -1;
	}

	i_msg = 0;
  	while (!stop)
	{
		chars_read = 0;
		while ((ret_val = read (chardev_FD, chardev_buffer, BYTES)) != 0)
			chars_read += ret_val;	// read the driver until EOF
		chardev_buffer[chars_read] = '\0';	// NULL terminate
		printf ("%s", chardev_buffer);

		sprintf (new_msg, "New message %d\n", i_msg);
		i_msg++;
		write (chardev_FD, new_msg, strlen(new_msg));

		sleep (1);
	}
	close (chardev_FD);
   return 0;
}
