#include "../../Math.hpp"
#include "Voice.hpp"

Sound::Synth::Voice::Voice(bool Connection,Sound::Synth::Operator Modulator,Sound::Synth::Operator Carrier,float Frequency,float Volume,unsigned char Note) :
Connection(Connection) ,
Modulator(Modulator) ,
Carrier(Carrier) ,
ModulatorOffset(0.f) ,
CarrierOffset(0.f) ,
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
		ModulatorOffset += Frequency * Modulator.Multiplier / 11025;
		CarrierOffset += Frequency * Carrier.Multiplier / 11025;
		while (ModulatorOffset >= 1)
		{
			ModulatorOffset -= 1;
		}
		while (CarrierOffset >= 1)
		{
			CarrierOffset -= 1;
		}
		Sound::Synth::Envelope(Modulator,&ModulatorStage,&ModulatorEnvelope);
		Sound::Synth::Envelope(Carrier,&CarrierStage,&CarrierEnvelope);
		if (Connection)
		{
			CarrierOffset += ModulatorEnvelope * Sound::Synth::Wave(ModulatorOffset,Modulator.WaveformType);
		}
		else
		{
			Output[i] += Volume * ModulatorEnvelope * Sound::Synth::Wave(ModulatorOffset,Modulator.WaveformType);
		}
		Output[i] += Volume * CarrierEnvelope * Sound::Synth::Wave(CarrierOffset,Carrier.WaveformType);
		if (CarrierStage == Sound::Synth::ADSR_DEAD)
		{
			return i + 1;
		}
	}
	return OutLength;
}
