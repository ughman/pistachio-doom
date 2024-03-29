#ifndef Video_NullSystem_hpp
#define Video_NullSystem_hpp

#include "System.hpp"

namespace Video
{
	class NullSystem : public System
	{
	private:
		NullSystem(const NullSystem &);
		NullSystem &operator=(const NullSystem &);
	public:
		NullSystem();

		virtual void Write(unsigned char *Buffer);
		virtual void SetPalette(unsigned char *Palette);

		virtual bool Update(Event *E);

		virtual ~NullSystem();
	};
}

#endif
