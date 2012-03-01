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

		PtrList <Stream> Sounds;
		PtrList <Stream> Musics;
	public:
		void Callback(signed short *Stream,int OutLength);

		SDLSystem(float SoundVolume,float MusicVolume);

		virtual void PlaySound(Stream *S,void *UserData);
		virtual void PlayMusic(Stream *S,void *UserData);
		virtual void StopSound(void *UserData);
		virtual void StopMusic(void *UserData);
		virtual void StopAll();

		virtual void Update();

		virtual ~SDLSystem();
	};
}

#endif
