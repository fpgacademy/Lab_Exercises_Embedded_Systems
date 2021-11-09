Part1 is an executable file and example solution to Part I. It continuously prints values
produced by ADC channel 0. The program automatically exits after 12 seconds.

Signal_generator.ko is an example solution for the kernel module from Part II. It uses a timer 
and responds to interrupts to toggle the value of a signal written to pin D0
on connector JP1 (also know as port GPIO 0), creating a square wave on this pin. 

Part3 implements a simple oscilloscope. It displays a waveform of the signal read from the ADC. 
The program assumes that the signal generator kernel signal_generator.ko is running to provide 
the waveform, and that pin D0 of connector GPIO 0 is connected by a wire to channel 0 of the ADC.
If switch SW[0] = 1, the oscilloscope triggers on a positive edge of the square wave, else on 
a negative edge. The driver /dev/IntelFPGUP/video is used to draw waveforms on the VGA display.

The part3 program automatically exits after 30 seconds.

Part4 is the same as part3, except that the sweep time of the oscilloscope can be changed. The
default sweep time is 100 ms; pressing KEY[0] increases the sweep time by 100 ms, and pressing
KEY[1] decreases the sweep time by 100 ms.

The part4 program automatically exits after 30 seconds.
