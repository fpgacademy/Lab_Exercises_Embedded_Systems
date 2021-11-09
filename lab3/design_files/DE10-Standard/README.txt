Part4 is an executable file and example solution to Part IV. It requires the drivers 
/dev/IntelFPGAUP/SW, dev/IntelFPGAUP/KEY, /dev/IntelFPGAUP/LEDR, and /dev/IntelFPGAUP/HEX.
These drivers can be found in /home/root/Linux_Libraries/drivers.

In the part4 program, whenever a pushbutton KEY is pressed the SW switches are copied to the
LEDR lights. Also, a running accumulation is computed of the values of the SW switches each 
time a KEY is pressed, and shown on the HEX displays.

The program automatically exits after 12 seconds.
