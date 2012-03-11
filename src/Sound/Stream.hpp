#ifndef Sound_Stream_hpp
#define Sound_Stream_hpp

#include "../List.hpp"
#include "../Vector.hpp"
#include "Synth/Instrument.hpp"

namespace Sound
{
	class Stream
	{
	private:
		Stream(const Stream &);
		Stream &operator=(const Stream &);
	public:
		typedef Stream *(*Format)(unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);

		static List <Format> Formats;

		static Stream *Load(unsigned char *Data,size_t Length,const Vector <Synth::Instrument> &Instruments);

		void *UserData;

		Stream(void *UserData) : UserData(UserData) {}

		virtual size_t Play(float *Output,size_t OutLength) = 0;
		virtual Stream *Copy(void *UserData) = 0;

		virtual ~Stream() {}
	};
}

#endif
