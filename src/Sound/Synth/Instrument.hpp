#ifndef Sound_Synth_Instrument_hpp
#define Sound_Synth_Instrument_hpp

#include "Operator.hpp"

namespace Sound
{
	namespace Synth
	{
		struct Instrument
		{
			bool FixedPitch;
			bool DoubleVoice;
			unsigned char Note;
			float Tuning[2];
			Operator Modulator[2];
			float Feedback[2];
			Operator Carrier[2];
			bool Connection[2];
			short Offset[2];
		};
	}
}

#endif
