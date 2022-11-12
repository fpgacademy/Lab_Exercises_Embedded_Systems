#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "wrappers.h"

/* This file contains wrappers for the stopwatch device driver */
int stopwatch_FD = -1;

#define stopwatch_CHAR_DEV "/dev/stopwatch"

#define STOPWATCH_BYTES 9           // number of bytes to read from stopwatch device

/* Declare buffers for device drivers */
#define BUF_SIZE 32                 // reserve some buffer space
char stopwatch_buffer[BUF_SIZE];    // buffer for stopwatch data

/**
 * Function stopwatch_open: opens the stopwatch device. 
 * Return: 1 on success, else 0
 */
int stopwatch_open( ) {
    if ((stopwatch_FD = open (stopwatch_CHAR_DEV, O_RDWR)) < 0) {
        fprintf(stderr, "ERROR: Failed to open %s\n", stopwatch_CHAR_DEV);
        return 0;
    }
    else
        return 1;
}

/**
 * Function stopwatch_read: reads the stopwatch device
 * Parameter MM: pointer for minutes (00 - 59)
 * Parameter SS: pointer for seconds (00 - 59)
 * Parameter DD: pointer for hundredths (00 - 99)
 * Return: 1 on success, else 0
 */
int stopwatch_read(int *MM, int *SS, int *DD) {
    int bytes, bytes_read;
    bytes_read = 0;
    if (stopwatch_FD > 0) {
        while ((bytes = read (stopwatch_FD, stopwatch_buffer, STOPWATCH_BYTES)) > 0)
            bytes_read += bytes;    // read the port until EOF
        if (bytes_read != STOPWATCH_BYTES) {
            fprintf (stderr, "Error: %d bytes expected from %s, but %d bytes read\n", 
                STOPWATCH_BYTES, stopwatch_CHAR_DEV, bytes_read);
            return 0;
        }
        if (sscanf (stopwatch_buffer, "%2d:%2d:%2d", MM, SS, DD) != 3) {
            fprintf (stderr, "Can't parse data from %s: %s\n", stopwatch_CHAR_DEV, 
                stopwatch_buffer);
            return 0;
        }
    }
    else {          
        fprintf (stderr, "Can't read %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
        return 0;
    }
    return 1;
}

/**
 * Function stopwatch_stop: stops the stopwatch device
 * Return: void
 */
void stopwatch_stop( ) {
    if (stopwatch_FD > 0)
        write (stopwatch_FD, "stop", 4);    // stop stopwatch
    else
        fprintf (stderr, "Can't write %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
}

/**
 * Function stopwatch_run: runs the stopwatch device
 * Return: void
 */
void stopwatch_run( ) {
    if (stopwatch_FD > 0)
        write (stopwatch_FD, "run", 3); // stop stopwatch
    else
        fprintf (stderr, "Can't write %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
}

/**
 * Function stopwatch_display: turns on 7-segment display for the stopwatch device
 * Return: void
 */
void stopwatch_display( ) {
    if (stopwatch_FD > 0)
        write (stopwatch_FD, "disp", 4);    // turn on stopwatch display
    else
        fprintf (stderr, "Can't write %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
}

/**
 * Function stopwatch_nodisplay: turns off 7-segment display for the stopwatch device
 * Return: void
 */
void stopwatch_nodisplay( ) {
    if (stopwatch_FD > 0)
        write (stopwatch_FD, "nodisp", 6);    // turn off stopwatch display
    else
        fprintf (stderr, "Can't write %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
}

/**
 * Function stopwatch_set: initializes the time for the stopwatch device
 * Parameter MM: minutes (00 - 59)
 * Parameter SS: seconds (00 - 59)
 * Parameter DD: hundredths (00 - 99)
 * Return: void
 */
void stopwatch_set(int MM, int SS, int DD) {
    if (stopwatch_FD > 0) {
        sprintf (stopwatch_buffer, "%02d:%02d:%02d", MM, SS, DD);
        write (stopwatch_FD, stopwatch_buffer, strlen(stopwatch_buffer));
    }
    else
        fprintf (stderr, "Can't write %s: stopwatch_FD = %d\n", stopwatch_CHAR_DEV, stopwatch_FD);
}

/**
 * Function stopwatch_close: closes the stopwatch device
 * Return: void
 */
void stopwatch_close( ) {
    if (stopwatch_FD > 0)
        close (stopwatch_FD);
}
