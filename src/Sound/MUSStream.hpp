#ifndef Sound_MUSStream_hpp
#define Sound_MUSStream_hpp

#include "../Vector.hpp"
#include "Stream.hpp"
#include "Synth/Channel.hpp"
#include "Synth/Instrument.hpp"

namespace Sound
{
	class MUSStream : public Stream
	{
	private:
		MUSStream(const MUSStream &);
		MUSStream &operator=(const MUSStream &);

		Vector <unsigned char> Buffer;
		Vector <Synth::Instrument> Instruments;
		Synth::Channel Channels[16];
		float TimeRemaining;
	public:
		explicit MUSStream(void *UserData,unsigned char *Buffer,size_t Length,const Vector <Synth::Instrument> &Instruments);

		virtual size_t Play(float *Output,size_t OutLength);
		virtual MUSStream *Copy(void *UserData);

		virtual ~MUSStream();
	};
}

#endif
