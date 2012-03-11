#include <limits.h>

#include "../Math.hpp"
#include "../Vector.hpp"
#include "../Memory.hpp"
#include "../Exception.hpp"
#include "SDLSystem.hpp"

class SDLAudioLock
{
public:
	SDLAudioLock()
	{
		SDL_LockAudio();
	}

	~SDLAudioLock()
	{
		SDL_UnlockAudio();
	}
};

static void AudioCallback(void *Sys,unsigned char *Output,int OutLength)
{
	((Sound::SDLSystem *)Sys)->Callback((signed short *)Output,OutLength / 2);
}

void Sound::SDLSystem::Callback(signed short *Output,int OutLength)
{
	Vector <float> SoundOutput(OutLength);
	Vector <float> MusicOutput(OutLength);
	for (size_t i = 0;i < OutLength;i++)
	{
		// No bounds checking
		SoundOutput.Array[i] = 0.f;
		MusicOutput.Array[i] = 0.f;
	}
	for (PtrLink <Stream> *it = Sounds.Front;it;)
	{
		if (it->Value->Play(SoundOutput.Array,OutLength) > 0)
		{
			it = it->Next;
		}
		else
		{
			it = it->Remove();
		}
	}
	for (PtrLink <Stream> *it = Musics.Front;it;)
	{
		if (it->Value->Play(MusicOutput.Array,OutLength) > 0)
		{
			it = it->Next;
		}
		else
		{
			it = it->Remove();
		}
	}
	float SoundLimit = 128.f;
	float MusicLimit = 512.f;
	for (size_t i = 0;i < OutLength;i++)
	{
		if (Math::Absolute(SoundOutput.Array[i]) > SoundLimit)
		{
			SoundLimit = Math::Absolute(SoundOutput.Array[i]);
		}
		if (Math::Absolute(MusicOutput.Array[i]) > MusicLimit)
		{
			MusicLimit = Math::Absolute(MusicOutput.Array[i]);
		}
	}
	for (size_t i = 0;i < OutLength;i++)
	{
		Output[i]  = SoundOutput.Array[i] * SHRT_MAX / 2 / SoundLimit * SoundVolume;
		Output[i] += MusicOutput.Array[i] * SHRT_MAX / 2 / MusicLimit * MusicVolume;
	}
}

Sound::SDLSystem::SDLSystem(float SoundVolume,float MusicVolume) :
Sound::System(SoundVolume,MusicVolume)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
	{
		throw StrException("SDL audio initialization failed.");
	}
	try
	{
		SDL_AudioSpec Spec;
		Spec.freq = 11025;
		Spec.format = AUDIO_S16SYS;
		Spec.channels = 1;
		Spec.samples = 256;
		Spec.callback = AudioCallback;
		Spec.userdata = this;
		if (SDL_OpenAudio(&Spec,0) == -1)
		{
			throw StrException("SDL audio mode setting failed.");
		}
		SDL_PauseAudio(false);
	}
	catch (...)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		throw;
	}
}

void Sound::SDLSystem::PlaySound(Sound::Stream *S,void *UserData)
{
	using (Sound::Stream *NS = S->Copy(UserData))
	{
		SDLAudioLock SAL;
		Sounds.Add(NS);
	}
	end_using(NS);
}

void Sound::SDLSystem::PlayMusic(Sound::Stream *S,void *UserData)
{
	using (Sound::Stream *NS = S->Copy(UserData))
	{
		SDLAudioLock SAL;
		Musics.Add(NS);
	}
	end_using(NS);
}

void Sound::SDLSystem::StopSound(void *UserData)
{
	SDLAudioLock SAL;
	for (PtrLink <Stream> *it = Sounds.Front;it;)
	{
		if (it->Value->UserData == UserData)
		{
			it = it->Remove();
		}
		else
		{
			it = it->Next;
		}
	}
}

void Sound::SDLSystem::StopMusic(void *UserData)
{
	SDLAudioLock SAL;
	for (PtrLink <Stream> *it = Musics.Front;it;)
	{
		if (it->Value->UserData == UserData)
		{
			it = it->Remove();
		}
		else
		{
			it = it->Next;
		}
	}
}

void Sound::SDLSystem::StopAll()
{
	SDLAudioLock SAL;
	Sounds.Clear();
	Musics.Clear();
}

void Sound::SDLSystem::Update()
{
}

Sound::SDLSystem::~SDLSystem()
{
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
