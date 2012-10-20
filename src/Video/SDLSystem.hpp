#ifndef Video_SDLSystem_hpp
#define Video_SDLSystem_hpp

#include <SDL.h>

#include "System.hpp"

namespace Video
{
	class SDLSystem : public System
	{
	private:
		SDLSystem(const SDLSystem &);
		SDLSystem &operator=(const SDLSystem &);

		SDL_Surface *Screen;

		int Width;
		int Height;
	public:
		explicit SDLSystem(int Width,int Height,bool Fullscreen);

		virtual void Write(unsigned char *Buffer);
		virtual void SetPalette(unsigned char *Palette);

		virtual bool Update(Event *E);

		virtual ~SDLSystem();
	};
}

#endif
