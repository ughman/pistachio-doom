#ifndef Sound_PCMStream_hpp
#define Sound_PCMStream_hpp

#include "../Vector.hpp"
#include "Stream.hpp"

namespace Sound
{
	class PCMStream : public Stream
	{
	private:
		PCMStream(const PCMStream &);
		PCMStream &operator=(const PCMStream &);

		float Offset;
		float SampleRate;
		Vector <signed char> Buffer;
	public:
		explicit PCMStream(void *UserData,float SampleRate,signed char *Buffer,size_t Length);
		explicit PCMStream(void *UserData,float SampleRate,unsigned char *Buffer,size_t Length);

		virtual size_t Play(float *Output,size_t OutLength);
		virtual PCMStream *Copy(void *UserData);

		virtual ~PCMStream();
	};
}

#endif
