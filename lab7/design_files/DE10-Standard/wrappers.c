#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "wrappers.h"

/* This file contains wrappers for several FPGA character device drivers */
int accel_FD = -1;

#define accel_CHAR_DEV "/dev/accel"

#define ACCEL_BYTES 21				// number of bytes to read from accel device

/* Declare buffer for reading/writing data to device driver */
#define BUF_SIZE 32							// large enough for all drivers
char accel_buffer[BUF_SIZE];				// buffer for accel data

/**
 * Opens the 3D-accelerometer accel device
 * Return: 1 on success, else 0
 */
int accel_open( )
{
 	if ((accel_FD = open (accel_CHAR_DEV, O_RDWR)) < 0)
	{
		fprintf(stderr, "ERROR: Failed to open %s\n", accel_CHAR_DEV);
		return 0;
	}
	else
		return 1;
}

/**
 * Reads data from the 3D-accelerometer accel device
 * Parameter ready: pointer for returning ready signal. Set to 1 if new acceleration data
 * 	is available, else set to 0
 * Parameter tap: pointer for returning the tap signal. Set to 1 if a tap event has
 * 	occured, else set to 0
 * Parameter dtap: pointer for returning the double-tap signal. Set to 1 if a double-tap 
 * 	event has occured, else set to 0
 * Parameter x: pointer for returning acceleration data in the x direction
 * Parameter y: pointer for returning acceleration data in the y direction
 * Parameter z: pointer for returning acceleration data in the z direction
 * Parameter mg_per_lsb: pointer for returning the acceleration-data scale factor 
 * Return: 1 on success, else 0
 */
int accel_read(int *ready, int *tap, int *dtap, int *x, int *y, int *z, int *mg_per_lsb)
{
  	int bytes, bytes_read, int_source;
	bytes_read = 0;
	if (accel_FD > 0)
	{
		while ((bytes = read (accel_FD, accel_buffer, ACCEL_BYTES)) > 0)
			bytes_read += bytes;	// read the port until EOF
		if (bytes_read != ACCEL_BYTES)
		{
			fprintf (stderr, "Error: %d bytes expected from %s, but %d bytes read\n", ACCEL_BYTES, 
				accel_CHAR_DEV, bytes_read);
			return 0;
		}
		else if (sscanf (accel_buffer, "%X %d %d %d %d", &int_source, x, y, z, mg_per_lsb) != 5)
		{
			fprintf (stderr, "Can't parse data from %s: %s\n", accel_CHAR_DEV, accel_buffer);
			return 0;
		}
		// check the interrupt source bits
		*ready = 0; *tap = 0; *dtap = 0;
  		if (int_source & 0x20)
			*dtap = 1;
		if ((int_source & 0x40) || (int_source & 0x20))
			*tap = 1;
		if (int_source & 0x80)
			*ready = 1; 
	}
	else
	{		  
		fprintf (stderr, "Can't read %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
		return 0;
	}
	return 1;
}

/**
 * Initializes the 3D-acceleration device
 * Return: void
 */
void accel_init( )
{
	if (accel_FD > 0)
		write (accel_FD, "init", 4);		// initialize accel
	else
		fprintf (stderr, "Can't write %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
}

/**
 * Calibrates the 3D-acceleration device
 * Return: void
 */
void accel_calibrate( )
{
	if (accel_FD > 0)
		write (accel_FD, "calibrate", 9);		// calibrate the device
	else
		fprintf (stderr, "Can't write %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
}

/**
 * Request printing of the device ID from the 3D-acceleration device
 * Return: void
 */
void accel_device( )
{
	if (accel_FD > 0)
		write (accel_FD, "device", 6);		// request the device ID
	else
		fprintf (stderr, "Can't write %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
}

/**
 * Sets the format of acceleration data
 * Parameter full: a value of 1 sets full resolution
 * Parameter range: sets the G range, where G = {2, 4, 8, 16}
 * Return: void
 */
void accel_format(int full, int range)
{
	if (accel_FD > 0)
	{
		sprintf (accel_buffer, "format %d %d", full, range);
		write (accel_FD, accel_buffer, strlen(accel_buffer));
	}
	else
		fprintf (stderr, "Can't write %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
}

/**
 * Sets the data rate of acceleration data
 * Parameter rate: the data rate in Hz, where R = {25,12.5,6.25,1.56,0.78}
 * Return: void
 */
void accel_range(float rate)
{
	if (accel_FD > 0)
	{
		sprintf (accel_buffer, "rate %.2f", rate);
		write (accel_FD, accel_buffer, strlen(accel_buffer));
	}
	else
		fprintf (stderr, "Can't write %s: accel_FD = %d\n", accel_CHAR_DEV, accel_FD);
}

/**
 * Closes the 3-D acceleration device
 */
void accel_close( )
{
	if (accel_FD > 0)
		close (accel_FD);
}
