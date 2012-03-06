#ifndef Sound_MIDIStream_hpp
#define Sound_MIDIStream_hpp

#include "../List.hpp"
#include "../Vector.hpp"
#include "Stream.hpp"
#include "Synth/Channel.hpp"
#include "Synth/Instrument.hpp"

namespace Sound
{
	struct MIDITrack
	{
		Vector <unsigned char> Buffer;
		bool Waiting;
		float TimeRemaining;
		unsigned char Command;
		Synth::Channel Channels[16];

		explicit MIDITrack(unsigned char *Buffer,size_t Length) : Buffer(Buffer,Length),Waiting(false),TimeRemaining(0.f),Command(0) {}
	};

	class MIDIStream : public Stream
	{
	private:
		MIDIStream(const MIDIStream &);
		MIDIStream &operator=(const MIDIStream &);

		List <MIDITrack> Tracks;
		Vector <Synth::Instrument> Instruments;
		float TicksPerBeat;
		float BeatsPerSecond;
	public:
		explicit MIDIStream(void *UserData,unsigned char *Data,size_t Length,Vector <Synth::Instrument> Instruments,float TicksPerBeat,float BeatsPerSecond);
		explicit MIDIStream(void *UserData,List <MIDITrack> Tracks,Vector <Synth::Instrument> Instruments,float TicksPerBeat,float BeatsPerSecond);

		virtual size_t Play(float *Output,size_t OutLength);
		virtual MIDIStream *Copy(void *UserData);

		virtual ~MIDIStream();
	};
}

#endif
