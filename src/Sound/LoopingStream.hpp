#ifndef Sound_LoopingStream_hpp
#define Sonud_LoopingStream_hpp

#include "Stream.hpp"

namespace Sound
{
	class LoopingStream : public Stream
	{
	private:
		LoopingStream(const LoopingStream &);
		LoopingStream &operator=(const LoopingStream &);

		Stream *Source;
		Stream *Current;
	public:
		LoopingStream(void *UserData,Stream *S);

		virtual size_t Play(unsigned char *Output,size_t OutLength);
		virtual LoopingStream *Copy(void *UserData);

		virtual ~LoopingStream();
	};
}

#endif
