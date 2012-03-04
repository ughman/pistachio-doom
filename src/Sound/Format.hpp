#ifndef Sound_Format_hpp
#define Sound_Format_hpp

#include "../List.hpp"
#include "../Vector.hpp"
#include "Stream.hpp"
#include "Synth/Instrument.hpp"

namespace Sound
{
	typedef Sound::Stream *(*Format)(unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);

	extern List <Format> Formats;

	Sound::Stream *Load        (unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);

	Sound::Stream *LoadDoomWave(unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);
	Sound::Stream *LoadMUS     (unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);
}

#endif
