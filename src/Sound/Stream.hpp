#ifndef Sound_Stream_hpp
#define Sound_Stream_hpp

namespace Sound
{
	class Stream
	{
	private:
		Stream(const Stream &);
		Stream &operator=(const Stream &);
	public:
		void *UserData;

		Stream(void *UserData) : UserData(UserData) {}

		virtual size_t Play(unsigned char *Output,size_t OutLength) = 0;
		virtual Stream *Copy(void *UserData) = 0;

		virtual ~Stream() {}
	};
}

#endif
