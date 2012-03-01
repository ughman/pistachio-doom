#ifndef Sound_Synth_Operator_hpp
#define Sound_Synth_Operator_hpp

namespace Sound
{
	namespace Synth
	{
		enum WaveformType
		{
			WF_SINE,
			WF_HALFSINE,
			WF_ABSSINE,
			WF_SAWTOOTH
		};

		struct Operator
		{
			bool Tremolo;
			bool Vibrato;
			bool DoSustain;
			bool KSR;
			float Multiplier;
			float Attack;
			float Decay;
			float Sustain;
			float Release;
			WaveformType WaveformType;
		};

		enum ADSRStage
		{
			ADSR_ATTACK,
			ADSR_DECAY,
			ADSR_SUSTAIN,
			ADSR_RELEASE,
			ADSR_DEAD
		};

		void Envelope(Operator Op,ADSRStage *Stage,float *Envelope);
		float Wave(float Offset,WaveformType Type);
	}
}

#endif
