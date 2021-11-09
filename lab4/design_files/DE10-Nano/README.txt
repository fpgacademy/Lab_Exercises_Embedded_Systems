Part3 is an executable file and example solution to Part III. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, /dev/IntelFPGAUP/LEDR, and stopwatch.ko.
The /dev/IntelFPGAUP drivers can be found in /home/root/Linux_Libraries/drivers.

When executing the part3 program the stopwatch can be paused/run by pressing KEY0. Pressing 
KEY1 causes the stopwatch time to be displayed on the Terminal window. When it is stopped, 
each digit in the stopwatch can be set by pressing KEY1. Each time KEY1 is pressed, one digit 
in the stopwatch gets set, to the values of the SW switches. The digits are set in sequence 
from rightmost to leftmost. Pressing KEY0 concludes the setting procedure and runs the stopwatch.

The part3 program automatically exits after 30 seconds.

Part4 is an executable file and example solution to Part IV. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, /dev/IntelFPGAUP/LEDR, and stopwatch.ko.
The /dev/IntelFPGAUP drivers can be found in /home/root/Linux_Libraries/drivers.

The code implements a game that works as follows: in the first step a default stopwatch time 
of 00:10:00 is set by the program. The user can change the stopwatch time by using the SW 
switches and KEY1. Each time KEY1 is pressed, one digit in the stopwatch gets set, to the 
values of the SW switches. The digits are set in sequence from rightmost to leftmost. 
Pressing KEY0 concludes the setting procedure and starts the game.  The program then asks a 
series of math questions that the user needs to answer within the stopwatch time. After each 
successful question the stopwatch is reset. Questions become increasingly difficult over time.
At the end, when the user fails to respond within the stopwatch time, some stats are printed.

The part4 program automatically exits after 30 seconds.
