#include "Stream.hpp"

List <Sound::Stream::Format> Sound::Stream::Formats;

Sound::Stream *Sound::Stream::Load(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	for (Link <Sound::Stream::Format> *it = Formats.Front;it;it = it->Next)
	{
		if (Sound::Stream *Result = it->Value(Data,Length,Instruments))
		{
			return Result;
		}
	}
	throw StrException("Unknown audio format.");
}
