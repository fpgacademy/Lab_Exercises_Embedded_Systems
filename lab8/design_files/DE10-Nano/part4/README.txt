Part4 implements a simple digital piano. It reads from a USB keyboard device to see which piano
"keys" have been pressed. Keys are from Q to I and 2 to 7 (with some gaps). Multiple notes
can be played at the same time (chords). When a piano key (Q, etc) is struck, a waveform 
corresponding to the current sound is sent to the video display. The device
driver /dev/IntelFPGAUP/video is used for the display. This driver can be
found in /home/root/Linux_Libraries/drivers.

If the optional [-w] option is given, the program writes the sounds played to the file 
piano.wav, which is an MS Windows sound file.

The program automatically exits after 30 seconds. 
