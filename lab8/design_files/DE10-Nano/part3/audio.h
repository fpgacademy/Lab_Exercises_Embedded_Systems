#ifndef AUDIO_H_
#define AUDIO_H_

#define PI 3.14159265
#define PI2 6.28318531
#define SAMPLING_RATE 12000
#define MAX_VOLUME 0x7fffffff

// Audio Core Registers
#define FIFOSPACE 1
#define LDATA 2
#define RDATA 3

// Audio Functions
void Audio_Init();
void Audio_Play_Sinusoid(float freq, unsigned int length_msecs, unsigned int volume);
void Audio_Write_To_Buffer(int sample);
void Audio_Wait_For_Buffer_Space();

// Cleanup
void Audio_Close_Device();

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

#endif /*AUDIO_H_*/
