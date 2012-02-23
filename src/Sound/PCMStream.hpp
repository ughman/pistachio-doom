#ifndef Sound_PCMStream_hpp
#define Sound_PCMStream_hpp

#include "Stream.hpp"

namespace Sound
{
	class PCMStream : public Stream
	{
	private:
		PCMStream(const PCMStream &);
		PCMStream &operator=(const PCMStream &);

		unsigned char *Buffer;
		size_t Length;
	public:
		explicit PCMStream(void *UserData,unsigned char *Buffer,size_t Length);

		virtual size_t Play(unsigned char *Output,size_t OutLength);
		virtual PCMStream *Copy(void *UserData);

		virtual ~PCMStream();
	};
}

#endif
