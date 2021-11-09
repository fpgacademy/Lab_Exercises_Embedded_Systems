#ifndef accel_H
#define accel_H

/**
 * Opens the 3D-accelerometer accel device
 * Return: 1 on success, else 0
 */
int accel_open (void);

/**
 * Function: accel_read: reads data from the 3D-accelerometer accel device
 * Parameter ready: pointer for returning ready signal (1 if new data, else 0)
 * Parameter tap: pointer for returning the tap signal (1 if tap event, else 0)
 * Parameter dtap: pointer for the double-tap signal (1 if double-tap event, 
 * 	else 0)
 * Parameter x: pointer for returning acceleration data in the x direction
 * Parameter y: pointer for returning acceleration data in the y direction
 * Parameter z: pointer for returning acceleration data in the z direction
 * Parameter mg_per_lsb: pointer for returning the acceleration-data scale factor 
 * Return: 1 on success, else 0
 */
int accel_read (int * /*ready*/, int * /*tap*/, int * /*dtap*/, int * /*x*/, 
	int * /*y*/, int * /*z*/, int * /*mg_per_lsb*/);

/**
 * Function: accel_init: initializes the 3D-acceleration device
 * Return: void
 */
void accel_init (void);

/**
 * Function accel_calibrate: xalibrates the 3D-acceleration device
 * Return: void
 */
void accel_calibrate (void);

/**
 * Function accel_device: request printing of the device ID
 * Return: void
 */
void accel_device (void);

/**
 * Functio accel_format: sets the format of acceleration data
 * Parameter full: a value of 1 sets full resolution
 * Parameter range: sets the G range, where G = {2, 4, 8, 16}
 * Return: void
 */
void accel_format (int /*full*/, int /*range*/);

/**
 * Function accel_rate: sets the data rate of acceleration data
 * Parameter rate: the data rate in Hz, where R = {25,12.5,6.25,1.56,0.78}
 * Return: void
 */
void accel_rate (float /*rate*/);

/**
 * Function accel_close: closes the 3-D acceleration device
 * Return: void
 */
void accel_close (void);

#endif
