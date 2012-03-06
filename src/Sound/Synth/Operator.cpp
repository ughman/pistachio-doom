#include "../../Math.hpp"
#include "../../Exception.hpp"
#include "Operator.hpp"

void Sound::Synth::Envelope(Sound::Synth::Operator Op,Sound::Synth::ADSRStage *Stage,float *Envelope)
{
	switch (*Stage)
	{
	case Sound::Synth::ADSR_ATTACK:
		*Envelope += Op.Attack;
		if (*Envelope >= 1)
		{
			*Envelope = 1;
			*Stage = Sound::Synth::ADSR_DECAY;
		}
		break;
	case Sound::Synth::ADSR_DECAY:
		*Envelope -= Op.Decay * (1 - Op.Sustain);
		if (*Envelope <= Op.Sustain)
		{
			*Envelope = Op.Sustain;
			*Stage = Sound::Synth::ADSR_SUSTAIN;
		}
		break;
	case Sound::Synth::ADSR_SUSTAIN:
		*Envelope = Op.Sustain;
		break;
	case Sound::Synth::ADSR_RELEASE:
		*Envelope -= Op.Release;
		if (*Envelope <= 0)
		{
			*Envelope = 0;
			*Stage = Sound::Synth::ADSR_DEAD;
		}
		break;
	case Sound::Synth::ADSR_DEAD:
		*Envelope = 0;
		break;
	default:
		throw StrException("Unknown ADSR state.");
	}
}

float Sound::Synth::Wave(float Offset,Sound::Synth::WaveformType Type)
{
	float Result = Math::Sine(Offset * 6.28318);
	switch (Type)
	{
	case Sound::Synth::WF_SINE:
		return Result;
	case Sound::Synth::WF_HALFSINE:
		return (Result > 0) ? Result : 0.f;
	case Sound::Synth::WF_ABSSINE:
		return Math::Absolute(Result);
	case Sound::Synth::WF_SAWTOOTH:
		return ((Offset < .25) || ((Offset > .5) && (Offset < .75))) ? Result : 0;
	default:
		throw StrException("Unknown waveform type.");
	}
}
