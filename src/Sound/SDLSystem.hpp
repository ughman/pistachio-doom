#ifndef Sound_SDLSystem_hpp
#define Sound_SDLSystem_hpp

#include <SDL.h>

#include "../List.hpp"
#include "System.hpp"

namespace Sound
{
	class SDLSystem : public System
	{
	private:
		SDLSystem(const SDLSystem &);
		SDLSystem &operator=(const SDLSystem &);

		PtrList <Stream> Channels;
	public:
		void Callback(signed short *Stream,int OutLength);

		SDLSystem();

		virtual void Play(Stream *S,void *UserData);
		virtual void Stop(void *UserData);
		virtual void StopAll();

		virtual void Update();

		virtual ~SDLSystem();
	};
}

#endif
