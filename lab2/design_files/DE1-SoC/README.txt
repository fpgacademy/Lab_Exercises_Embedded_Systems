The file part1 is an ARM executable program that provides a sample solution for Part I. The
program automatically exits after 12 seconds.

The files physical.c and physical.h provide sample source code for handling virtual-to-physical
memory mapping. These files are useful for Part I

The file timer.ko is a kernel module that provides a sample solution for Part II

The file stopwatch.ko is a kernel module that provides a sample solution for Part III. It can 
be controlled by the KEY pushbuttons and SW switches. The stopwatch can be stopped/started by
pressing pushbutton KEY0. When the stopwatch is stopped, it can be set to a starting time by 
using KEY1, KEY2, and KEY3. Pressing KEY1 causes the hundredth to be set using the SW switches,
pressing KEY2 sets the seconds, and pressing KEY3 sets the minutes. The maximum values possible 
are 59 for minutes, 59 for seconds, and 99 for hundredths. The SW values are interpreted as a 
10-bit binary number.
