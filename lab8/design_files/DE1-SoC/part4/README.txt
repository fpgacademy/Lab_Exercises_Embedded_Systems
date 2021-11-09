Part4 implements a simple digital piano. It reads from a USB keyboard device to see which piano
"keys" have been pressed. Keys are from Q to I and 2 to 7 (with some gaps). Multiple notes
can be played at the same time (chords). When a piano key (Q, etc) is struck, a waveform 
corresponding to the current sound is sent to the VGA display. The device
driver /dev/IntelFPGAUP/video is used for the display. This driver can be
found in /home/root/Linux_Libraries/drivers

The program takes an optional arguments
   [keyboard-path] provides a path to the keyboard device. Example:
	   /dev/input/by-id/usb-Logitech_USB_Receiver-event-kbd

The program automatically exits after 30 seconds. 
