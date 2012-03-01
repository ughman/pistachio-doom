#include <limits.h>

#include "../Math.hpp"
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
	float *X = new float [OutLength];
	try
	{
		for (size_t i = 0;i < OutLength;i++)
		{
			X[i] = 0.f;
		}
		for (PtrLink <Stream> *it = Channels.Front;it;)
		{
			if (it->Value->Play(X,OutLength) > 0)
			{
				it = it->Next;
			}
			else
			{
				it = it->Remove();
			}
		}
		float Limit = 127.f;
		for (size_t i = 0;i < OutLength;i++)
		{
			if (Math::Absolute(X[i]) > Limit)
			{
				Limit = Math::Absolute(X[i]);
			}
		}
		for (size_t i = 0;i < OutLength;i++)
		{
			Output[i] = X[i] * SHRT_MAX / Limit;
		}
	}
	catch (...)
	{
		delete [] X;
		throw;
	}
	delete [] X;
}

Sound::SDLSystem::SDLSystem()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
	{
		throw Exception();
	}
	try
	{
		SDL_AudioSpec Spec;
		Spec.freq = 11025;
		Spec.format = AUDIO_S16SYS;
		Spec.channels = 1;
		Spec.samples = 128;
		Spec.callback = AudioCallback;
		if (SDL_OpenAudio(&Spec,0) == -1)
		{
			throw Exception();
		}
		SDL_PauseAudio(false);
	}
	catch (...)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		throw;
	}
}

void Sound::SDLSystem::Play(Sound::Stream *S,void *UserData)
{
	using (Sound::Stream *NS = S->Copy(UserData))
	{
		SDLAudioLock SAL;
		Channels.Add(NS);
	}
	end_using(NS);
}

void Sound::SDLSystem::Stop(void *UserData)
{
	SDLAudioLock SAL;
	for (PtrLink <Stream> *it = Channels.Front;it;)
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
	Channels.Clear();
}

void Sound::SDLSystem::Update()
{
}

Sound::SDLSystem::~SDLSystem()
{
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
