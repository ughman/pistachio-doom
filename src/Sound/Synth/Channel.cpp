#include "../../Math.hpp"
#include "Channel.hpp"

size_t Sound::Synth::Channel::Play(float *Output,size_t OutLength)
{
	for (Link <Voice> *it = Voices.Front;it;)
	{
		if (it->Value.Play(Output,OutLength) > 0)
		{
			it = it->Next;
		}
		else
		{
			it = it->Remove();
		}
	}
	return OutLength;
}

void Sound::Synth::Channel::StartNote(unsigned char Note,unsigned char FreqNote)
{
	float Frequency = 440 * Math::Power(2,(FreqNote - 40) / 12.f);
	Voices.Add(Voice(Instrument.Connection[0],Instrument.Feedback[0],Instrument.Modulator[0],Instrument.Carrier[0],Frequency + Instrument.Tuning[0],Volume,Note));
	if (Instrument.DoubleVoice)
	{
		Voices.Add(Voice(Instrument.Connection[1],Instrument.Feedback[1],Instrument.Modulator[1],Instrument.Carrier[1],Frequency + Instrument.Tuning[1],Volume,Note));
	}
}

void Sound::Synth::Channel::StopNote(unsigned char Note)
{
	for (Link <Voice> *it = Voices.Front;it;it = it->Next)
	{
		if (it->Value.Note == Note)
		{
			it->Value.Held = false;
		}
	}
}
