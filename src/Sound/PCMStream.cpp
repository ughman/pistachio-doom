#include <SDL.h>

#include "PCMStream.hpp"

Sound::Stream *Sound::PCMStream::LoadDoomWave(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	if (Length < 8) return 0;
	if (Data[0] != 3) return 0;
	if (Data[1] != 0) return 0;
	unsigned short SampleRate;
	SampleRate  = Data[2];
	SampleRate |= Data[3] << 8;
	return new Sound::PCMStream(0,SampleRate,Data + 8,Length - 8);
}

Sound::PCMStream::PCMStream(void *UserData,float SampleRate,signed char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Offset(0.f) ,
SampleRate(SampleRate) ,
Buffer(Buffer,Length)
{
}

Sound::PCMStream::PCMStream(void *UserData,float SampleRate,unsigned char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Offset(0.f) ,
SampleRate(SampleRate) ,
Buffer((signed char *)Buffer,Length)
{
	for (size_t i = 0;i < Length;i++)
	{
		this->Buffer[i] -= 128;
	}
}

size_t Sound::PCMStream::Play(float *Output,size_t OutLength)
{
	if (Buffer.Length == 0)
	{
		return 0;
	}
	size_t i;
	for (i = 0;i < OutLength;i++)
	{
		Output[i] += Buffer[0];
		Offset += SampleRate / 11025;
		while (Offset >= 1)
		{
			Offset -= 1;
			Buffer.Remove(0,1);
			if (Buffer.Length == 0)
			{
				return i + 1;
			}
		}
	}
	return OutLength;
}

Sound::PCMStream *Sound::PCMStream::Copy(void *UserData)
{
	return new Sound::PCMStream(UserData,SampleRate,Buffer.Array,Buffer.Length);
}

Sound::PCMStream::~PCMStream()
{
}
