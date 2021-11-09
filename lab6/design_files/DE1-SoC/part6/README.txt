Part6 is an executable file and example solution to Part VI. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, and /dev/IntelFPGAUP/video. These drivers
can be found in /home/root/Linux_Libraries/drivers.  The video driver /dev/IntelFPGAUP 
implements the same functionality as the one that you are supposed to write as part of 
the solution to Part 6. 

The code creates an animation of moving boxes and lines on the VGA screen. Pressing KEY0 
makes the animation move more quickly, and KEY1 slows it down. Pressing KEY2 deletes a box,
and KEY3 adds one.  If all SW switches are down, lines are drawn between boxes, otherwise 
only boxes are drawn. A running count of the number of video frames drawn is displayed in 
the upper-right corner of the screen. The program automatically exits after 30 seconds.
