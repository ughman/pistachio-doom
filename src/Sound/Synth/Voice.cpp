#include "../../Math.hpp"
#include "Voice.hpp"

Sound::Synth::Voice::Voice(bool Connection,float Feedback,Sound::Synth::Operator Modulator,Sound::Synth::Operator Carrier,float Frequency,float Volume,unsigned char Note) :
Connection(Connection) ,
Feedback(Feedback) ,
Modulator(Modulator) ,
Carrier(Carrier) ,
Time(0.f) ,
ModulatorStage(ADSR_ATTACK) ,
CarrierStage(ADSR_ATTACK) ,
ModulatorEnvelope(0.f) ,
CarrierEnvelope(0.f) ,
Frequency(Frequency) ,
Volume(Volume) ,
Note(Note) ,
Held(true)
{
}

size_t Sound::Synth::Voice::Play(float *Output,size_t OutLength)
{
	if (CarrierStage == Sound::Synth::ADSR_DEAD)
	{
		return 0;
	}
	if (!Held)
	{
		ModulatorStage = Sound::Synth::ADSR_RELEASE;
		CarrierStage = Sound::Synth::ADSR_RELEASE;
	}
	for (size_t i = 0;i < OutLength;i++)
	{
		Time += 1 / 11025.f;
		Sound::Synth::Envelope(Modulator,&ModulatorStage,&ModulatorEnvelope);
		Sound::Synth::Envelope(Carrier,&CarrierStage,&CarrierEnvelope);
		if (Connection)
		{
			Output[i] += Volume * CarrierEnvelope * Sound::Synth::Wave(Frequency * Carrier.Multiplier * Time + (ModulatorEnvelope * Sound::Synth::Wave(Frequency * Modulator.Multiplier * Time,Modulator.WaveformType)),Carrier.WaveformType);
		}
		else
		{
			Output[i] += Volume * ModulatorEnvelope * Sound::Synth::Wave(Frequency * Modulator.Multiplier * Time,Modulator.WaveformType);
			Output[i] += Volume * CarrierEnvelope * Sound::Synth::Wave(Frequency * Carrier.Multiplier * Time,Carrier.WaveformType);
		}
		if (CarrierStage == Sound::Synth::ADSR_DEAD)
		{
			return i + 1;
		}
	}
	return OutLength;
}
