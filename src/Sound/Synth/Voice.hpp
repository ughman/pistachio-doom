#ifndef Sound_Synth_Voice_hpp
#define Sound_Synth_Voice_hpp

#include "Operator.hpp"

namespace Sound
{
	namespace Synth
	{
		class Voice
		{
		private:
			bool Connection;
			float Feedback;
			Operator Modulator;
			Operator Carrier;
			float Time;
			ADSRStage ModulatorStage;
			ADSRStage CarrierStage;
			float ModulatorEnvelope;
			float CarrierEnvelope;
			float Frequency;
			float Volume;
		public:
			unsigned char Note;
			bool Held;

			explicit Voice(bool Connection,float Feedback,Operator Modulator,Operator Carrier,float Frequency,float Volume,unsigned char Note);

			size_t Play(float *Output,size_t OutLength);
		};
	}
}

#endif
