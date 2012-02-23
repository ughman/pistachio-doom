#ifndef Video_System_hpp
#define Video_System_hpp

#include "Event.hpp"

namespace Video
{
	class System
	{
	private:
		System(const System &);
		System &operator=(const System &);
	public:
		System() {}

		virtual void Read(unsigned char *Buffer) = 0;
		virtual void Write(unsigned char *Buffer,bool Update) = 0;
		virtual void SetPalette(unsigned char *Palette) = 0;

		virtual bool Update(Event *E) = 0;

		virtual ~System() {}
	};
}

#endif
