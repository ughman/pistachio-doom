#include <SDL.h>

#include "PCMStream.hpp"

Sound::PCMStream::PCMStream(void *UserData,signed char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Buffer(Buffer,Length)
{
}

Sound::PCMStream::PCMStream(void *UserData,unsigned char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
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
	else if (Buffer.Length <= OutLength)
	{
		size_t i;
		for (i = 0;i < Buffer.Length;i++)
		{
			Output[i] += Buffer[i];
		}
		Buffer.Remove(0,Buffer.Length);
		return i;
	}
	else
	{
		for (size_t i = 0;i < OutLength;i++)
		{
			Output[i] += Buffer[i];
		}
		Buffer.Remove(0,OutLength);
		return OutLength;
	}
}

Sound::PCMStream *Sound::PCMStream::Copy(void *UserData)
{
	return new Sound::PCMStream(UserData,Buffer.Array,Buffer.Length);
}

Sound::PCMStream::~PCMStream()
{
}
