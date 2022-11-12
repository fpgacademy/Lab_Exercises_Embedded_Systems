The directory part1 has the files needed for Part I of the exercise.

KEY_SW.ko is a character device driver that gives a sample solution for Part II.
LED.ko is a character device driver that gives a sample solution for Part III.

Part4 is an executable file and example solution to Part IV. It requires the drivers 
SW_KEY and LED.

In the part4 program, whenever a pushbutton KEY is pressed the SW switches are copied to the
LEDR lights. Also, a running accumulation is computed of the values of the SW switches each 
time a KEY is pressed, and shown on the HEX displays. The program automatically exits after 
a short time period.
