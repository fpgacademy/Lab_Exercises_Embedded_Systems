#include <stdio.h>

FILE *wav_open(char *);
int wav_header(int, int);
int wav_data(signed short);
void wav_close( );
