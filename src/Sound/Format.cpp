#include "../Exception.hpp"
#include "Format.hpp"
#include "PCMStream.hpp"
#include "MUSStream.hpp"
#include "MIDIStream.hpp"

List <Sound::Format> Sound::Formats;

Sound::Stream *Sound::Load(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	for (Link <Sound::Format> *it = Formats.Front;it;it = it->Next)
	{
		if (Sound::Stream *Result = it->Value(Data,Length,Instruments))
		{
			return Result;
		}
	}
	throw StrException("Unknown audio format.");
}

Sound::Stream *Sound::LoadDoomWave(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	if (Length < 8) return 0;
	if (Data[0] != 3) return 0;
	if (Data[1] != 0) return 0;
	unsigned short SampleRate;
	SampleRate  = Data[2];
	SampleRate |= Data[3] << 8;
	return new Sound::PCMStream(0,SampleRate,Data + 8,Length - 8);
}

Sound::Stream *Sound::LoadMUS(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	if (Length < 16) return 0;
	if (Data[0] != 'M') return 0;
	if (Data[1] != 'U') return 0;
	if (Data[2] != 'S') return 0;
	if (Data[3] != 0x1A) return 0;
	unsigned short InstrumentCount;
	InstrumentCount  = Data[12];
	InstrumentCount |= Data[13] << 8;
	if (Length < 16 + InstrumentCount * 2) throw StrException("Invalid MUS file.");
	return new Sound::MUSStream(0,Data + 16 + InstrumentCount * 2,Length - 16 - InstrumentCount * 2,Instruments);
}

Sound::Stream *Sound::LoadMIDI(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	if (Length < 14) return 0;
	if (Data[0] != 'M') return 0;
	if (Data[1] != 'T') return 0;
	if (Data[2] != 'h') return 0;
	if (Data[3] != 'd') return 0;
	if (Data[4] != 0) StrException("Invalid MIDI file.");
	if (Data[5] != 0) StrException("Invalid MIDI file.");
	if (Data[6] != 0) StrException("Invalid MIDI file.");
	if (Data[7] != 6) StrException("Invalid MIDI file.");
	unsigned short TimeDivision;
	TimeDivision  = Data[12] << 8;
	TimeDivision |= Data[13];
	if (TimeDivision & 0x8000)
	{
		unsigned char FPS = (TimeDivision & 0x7F00) >> 8;
		unsigned char TPF = TimeDivision & 0xFF;
		return new Sound::MIDIStream(0,Data + 14,Length - 14,Instruments,TPF,FPS);
	}
	else
	{
		return new Sound::MIDIStream(0,Data + 14,Length - 14,Instruments,TimeDivision,2);
	}
}
