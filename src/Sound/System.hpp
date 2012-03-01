#ifndef Sound_System_hpp
#define Sound_System_hpp

#include "Stream.hpp"

namespace Sound
{
	class System
	{
	private:
		System(const System &);
		System &operator=(const System &);
	public:
		float SoundVolume;
		float MusicVolume;

		System(float SoundVolume,float MusicVolume) : SoundVolume(SoundVolume),MusicVolume(MusicVolume) {}

		virtual void PlaySound(Stream *S,void *UserData) = 0;
		virtual void PlayMusic(Stream *S,void *UserData) = 0;
		virtual void StopSound(void *UserData) = 0;
		virtual void StopMusic(void *UserData) = 0;
		virtual void StopAll() = 0;

		virtual void Update() = 0;

		virtual ~System() {}
	};
}

#endif
