#include <SDL.h>

#include "../Memory.hpp"
#include "PCMStream.hpp"

Sound::PCMStream::PCMStream(void *UserData,signed char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Buffer((signed char *)Buffer) ,
Length(Length)
{
}

Sound::PCMStream::PCMStream(void *UserData,unsigned char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Buffer((signed char *)Buffer) ,
Length(Length)
{
	for (size_t i = 0;i < Length;i++)
	{
		Buffer[i] -= 128;
	}
}

size_t Sound::PCMStream::Play(float *Output,size_t OutLength)
{
	if (Length == 0)
	{
		return 0;
	}
	else if (Length <= OutLength)
	{
		for (size_t i = 0;i < Length;i++)
		{
			Output[i] += Buffer[i];
		}
		Length = 0;
		Memory::Free(Buffer);
		Buffer = 0;
		return Length;
	}
	else
	{
		for (size_t i = 0;i < OutLength;i++)
		{
			Output[i] += Buffer[i];
		}
		Memory::Move(Buffer,Buffer + OutLength,Length - OutLength);
		Buffer = (signed char *)Memory::Reallocate(Buffer,Length - OutLength);
		Length -= OutLength;
		return OutLength;
	}
}

Sound::PCMStream *Sound::PCMStream::Copy(void *UserData)
{
	using (signed char *NewBuffer = new signed char [Length])
	{
		Memory::Copy(NewBuffer,Buffer,Length);
		return new Sound::PCMStream(UserData,NewBuffer,Length);
	}
	end_using_array(NewBuffer);
}

Sound::PCMStream::~PCMStream()
{
	Memory::Free(Buffer);
}
