Part3 is an executable file and example solution to Part III. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, /dev/IntelFPGAUP/LEDR, and stopwatch.ko.
The /dev/IntelFPGAUP drivers can be found in /home/root/Linux_Libraries/drivers.

When executing the part3 program the stopwatch can be paused/run by pressing KEY0. The time 
can be set by using KEY1, KEY2, or KEY3. Pressing KEY1 sets the hundredths digits to the value
of SW, pressing KEY2 sets the seconds, and pressing KEY3 sets the minutes. The SW values are 
treated as 2-digit binary-code decimal (BCD) numbers.

The part3 program automatically exits after 30 seconds.

Part4 is an executable file and example solution to Part IV. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, /dev/IntelFPGAUP/LEDR, and stopwatch.ko.
The /dev/IntelFPGAUP drivers can be found in /home/root/Linux_Libraries/drivers.

The code implements a game that works as follows: in the first step a default stopwatch time 
is shown on the seven-segment displays, and the user can change the displayed time by using 
the SW switches and KEYs. Pressing KEY1 changes the hundredths part of the time, pressing 
KEY2 changes the seconds, and pressing KEY3 changes the minutes.  Pressing KEY0 starts the 
game. At this point the program prints a message and waits for the user to press the return key.
Following this action, the program asks a series of math questions that the user needs to 
answer within the stopwatch time. After each successful question the stopwatch is reset. 
Questions become increasingly difficult over time. At the end, when the user fails to respond 
within the stopwatch time, some stats are printed. 

The part4 program automatically exits after 30 seconds.
