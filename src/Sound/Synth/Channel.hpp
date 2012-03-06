#ifndef Sound_Synth_Channel_hpp
#define Sound_Synth_Channel_hpp

#include "../../List.hpp"
#include "Voice.hpp"
#include "Instrument.hpp"

namespace Sound
{
	namespace Synth
	{
		class Channel
		{
		public:
			List <Voice> Voices;
			Instrument Instrument;
			float Volume;

			Channel() : Volume(100) {}

			size_t Play(float *Output,size_t OutLength);

			void StartNote(unsigned char Note,unsigned char FreqNote);
			void StopNote(unsigned char Note);
		};
	}
}

#endif
