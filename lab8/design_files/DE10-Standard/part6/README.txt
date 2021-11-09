Part6 implements a simple digital piano. It reads from a keyboard device to see which piano
"keys" have been pressed. Keys are from Q to I and 2 to 7 (with some gaps). Multiple notes
can be played at the same time (chords). When a note is pressed a waveform corresponding 
to the current sound is sent to the VGA display. When the note is released the current 
waveform is drawn again (shows the amplitude changes from press to release). The driver 
/dev/IntelFPGAUP/LCD is used for the display, and /dev/IntelFPGAUP/audio is used for the audio. 
Pressing pushbutton KEY0 causes recording to start (up to 500 notes). Pressing KEY0 again stops 
the recording. Pressing KEY1 plays the recorded notes. The character device drivers 
/dev/IntelFPGAUP/KEY and /dev/IntelFPGAUP/LEDR are required, along with stopwatch. 
IntelFPGAUP drivers can be found in /home/root/Linux_Libraries/drivers.

Pressing KEY1 when the program first starts plays a pre-recorded "ditty".

The program takes two optional arguments:
   [-w] causes the program to save the tones played into the file piano.wav. This is an 
	   audio WAV file
   [keyboard-path] provides a path the keyboard device. Example:
	   /dev/input/by-id/usb-Logitech_USB_Receiver-event-kbd
	
The program automatically exits after 30 seconds. 

The files wav.c and wav.h can be used with any program to create a WAV file.
