#include <SDL.h>

#include "../Memory.hpp"
#include "PCMStream.hpp"

Sound::PCMStream::PCMStream(void *UserData,unsigned char *Buffer,size_t Length) :
Sound::Stream(UserData) ,
Buffer(Buffer) ,
Length(Length)
{
}

size_t Sound::PCMStream::Play(unsigned char *Output,size_t OutLength)
{
	if (Length == 0)
	{
		return 0;
	}
	else if (Length <= OutLength)
	{
		Memory::Copy(Output,Buffer,Length);
		Length = 0;
		Memory::Free(Buffer);
		Buffer = 0;
		return Length;
	}
	else
	{
		Memory::Copy(Output,Buffer,OutLength);
		Memory::Move(Buffer,Buffer + OutLength,Length - OutLength);
		Buffer = (unsigned char *)Memory::Reallocate(Buffer,Length - OutLength);
		Length -= OutLength;
		return OutLength;
	}
}

Sound::PCMStream *Sound::PCMStream::Copy(void *UserData)
{
	using (unsigned char *NewBuffer = new unsigned char [Length])
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
