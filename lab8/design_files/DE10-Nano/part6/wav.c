#include <stdio.h>
FILE				*wav_fp 			=NULL;				// audio file pointer
unsigned int 	wav_length 		=0;					// length of the data

// These are the required fields of the wav file header
char				*ChunkID			="RIFF";
unsigned int	ChunkSize		=0x00000000;		// placeholder; will be 36 + data length
char				*Format			="WAVE";
char				*Subchunk1ID	="fmt ";
unsigned int	Subchink1Size	=0x00000010;		// size = 16
unsigned short	AudioFormat		=0x0001;
unsigned short	NumChannels		=0x0001;
unsigned int	SampleRate		=0x00000000;		// placeholder for samples per second
unsigned int	ByteRate			=0x00000000;		// placeholder for bytes per second
unsigned short	BlockAlign		=0x0002;
unsigned short	BitsPerSample	=0x0010;				// 16 bits per sample
char				*Subchunk2ID	="data";
unsigned int	Subchunk2Size	=0x00000000;		// placeholder; will be data length

// Each data sample will be a signed 16-bit integer
signed short	DataSample		=0x0000;				// placeholder

FILE *wav_open(char *wav_filename)
{
  wav_fp = fopen (wav_filename, "wb");
  return (wav_fp);
}

int wav_header(int length, int rate)
{
	ChunkSize = length + 36;
	SampleRate = rate;
	ByteRate = rate * 2;
	Subchunk2Size = length;

	if (wav_fp == NULL)
		return (0);
	fflush (wav_fp);
	rewind (wav_fp);
	fwrite (ChunkID, sizeof(unsigned char), 4, wav_fp); 
	fwrite (&ChunkSize, sizeof(unsigned int), 1, wav_fp); 
	fwrite (Format, sizeof(unsigned char), 4, wav_fp); 
	fwrite (Subchunk1ID, sizeof(unsigned char), 4, wav_fp); 
	fwrite (&Subchink1Size, sizeof(unsigned int), 1, wav_fp); 
	fwrite (&AudioFormat, sizeof(unsigned short), 1, wav_fp); 
	fwrite (&NumChannels, sizeof(unsigned short), 1, wav_fp); 
	fwrite (&SampleRate, sizeof(unsigned int), 1, wav_fp); 
	fwrite (&ByteRate, sizeof(unsigned int), 1, wav_fp); 
	fwrite (&BlockAlign, sizeof(unsigned short), 1, wav_fp); 
	fwrite (&BitsPerSample, sizeof(unsigned short), 1, wav_fp); 
	fwrite (Subchunk2ID, sizeof(unsigned char), 4, wav_fp); 
	fwrite (&Subchunk2Size, sizeof(unsigned int), 1, wav_fp); 
	return (1);
}

int wav_data(signed short sample)
{
	DataSample = sample;
	if (wav_fp == NULL)
		return (0);
	fwrite (&DataSample, sizeof(signed short), 1, wav_fp); 
	return (1);
}

void wav_close( )
{
	fclose (wav_fp);
}

