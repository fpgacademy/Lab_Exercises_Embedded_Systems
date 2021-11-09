The file part3 is an executable program. It is an example solution to Part 3.  The piano tones
are played on the audio device of the DE10-Standard board.

The program takes two optional arguments:
   [-w] causes the program to save the tones played into the file piano.wav. This is an 
	   audio WAV file
   [keyboard-path] provides a path to the keyboard device. Example:
	   /dev/input/by-id/usb-Logitech_USB_Receiver-event-kbd

The files wav.c and wav.h can be used in any program to create a WAV file.
