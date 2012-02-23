#include "LoopingStream.hpp"

Sound::LoopingStream::LoopingStream(void *UserData,Stream *S) :
Sound::Stream(UserData) ,
Source(S),
Current(0)
{
}

size_t Sound::LoopingStream::Play(float *Output,size_t OutLength)
{
	size_t Result = OutLength;
	size_t Played = 0;
	if (!Current)
	{
		Current = Source->Copy(0);
	}
playloop:
	Played = Current->Play(Output,OutLength);
	OutLength -= Played;
	Output += Played;
	if (OutLength)
	{
		delete Current;
		Current = 0;
		Current = Source->Copy(0);
		goto playloop;
	}
	return Result;
}

Sound::LoopingStream *Sound::LoopingStream::Copy(void *UserData)
{
	using (Sound::Stream *S = Source->Copy(0))
	{
		return new Sound::LoopingStream(UserData,S);
	}
	end_using(S);
}

Sound::LoopingStream::~LoopingStream()
{
	delete Source;
	delete Current;
}
