part1: ARM executable, sample solution for Part I. The default location of the message on 
the screen is row 2, column 38. But the location can be specified on the command line. For 
example to show the message at row 10, column 20: ./part1 10 20. The code automatically 
exits after 12 seconds.

The files physical.c and physical.h provide sample source code for handling virtual-to-physical
memory mapping. They are useful for developing a solution to Part I

timer.ko: kernel module, sample solution for Part II. Pressing KEY1 prints the timer value on 
the Terminal window

stopwatch.ko: kernel module, sample solution for Part III. Can be controlled by KEY pushbuttons
and SW switches: The stopwatch can be paused/run by pressing pushbutton KEY0. Pressing KEY1 
causes the stopwatch time to be displayed on the Terminal window. When it is stopped, each 
digit in the stopwatch can be set by pressing KEY1. Each time KEY1 is pressed, one digit in 
the stopwatch gets set, to the values of the SW switches. The digits are set in sequence from 
rightmost to leftmost. Pressing KEY0 concludes the setting procedure and runs the stopwatch.
The SW switches are interpreted as a 10-bit binary value.
