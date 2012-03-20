#ifndef Sound_NullSystem_hpp
#define Sound_NullSystem_hpp

#include "System.hpp"

namespace Sound
{
	class NullSystem : public System
	{
	private:
		NullSystem(const NullSystem &);
		NullSystem &operator=(const NullSystem &);
	public:
		NullSystem(float SoundVolume,float MusicVolume);

		virtual void PlaySound(Stream *S,void *UserData);
		virtual void PlayMusic(Stream *S,void *UserData);
		virtual void StopSound(void *UserData);
		virtual void StopMusic(void *UserData);
		virtual void StopAll();

		virtual void Update();

		virtual ~NullSystem();
	};
}

#endif
