#include <limits.h>

#include "../Memory.hpp"
#include "../Exception.hpp"
#include "SDLSystem.hpp"

struct SDLStream
{
	size_t Length;
	unsigned char Data[];
};

static void AudioCallback(void *Sys,unsigned char *Output,int OutLength)
{
	((Sound::SDLSystem *)Sys)->Callback((signed short *)Output,OutLength / 2);
}

void Sound::SDLSystem::Callback(signed short *Output,int OutLength)
{
	size_t ChannelCount = Channels.Count();
	float *X = new float [OutLength];
	for (size_t i = 0;i < OutLength;i++)
	{
		X[i] = 0.f;
	}
	try
	{
		PtrLink <Sound::Stream> *Prev = 0;
		PtrLink <Sound::Stream> *This = Channels.Front;
		while (This)
		{
			if (!This->Value->Play(X,OutLength))
			{
				PtrLink <Sound::Stream> *Next = This->Next;
				This->Next = 0;
				delete This;
				This = Next;
				if (Prev)
				{
					Prev->Next = This;
				}
				else
				{
					Channels.Front = This;
				}
				continue;
			}
			Prev = This;
			This = This->Next;
		}
		float Limit = 256.f;
		for (size_t i = 0;i < OutLength;i++)
		{
			if (X[i] < -Limit)
			{
				Limit = -X[i];
			}
			else if (X[i] > Limit)
			{
				Limit = X[i];
			}
		}
		if (Limit > 0)
		{
			for (size_t i = 0;i < OutLength;i++)
			{
				Output[i] = X[i] * SHRT_MAX / Limit;
			}
		}
		else
		{
			for (size_t i = 0;i < OutLength;i++)
			{
				Output[i] = 0;
			}
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
		SDL_LockAudio();
		try
		{
			Channels.Add(NS);
		}
		catch (...)
		{
			SDL_UnlockAudio();
			throw;
		}
		SDL_UnlockAudio();
	}
	end_using(NS);
}

void Sound::SDLSystem::Stop(void *UserData)
{
	SDL_LockAudio();
	try
	{
		PtrLink <Sound::Stream> *Prev = 0;
		PtrLink <Sound::Stream> *This = Channels.Front;
		while (This)
		{
			if (This->Value->UserData == UserData)
			{
				PtrLink <Sound::Stream> *Next = This->Next;
				This->Next = 0;
				delete This;
				This = Next;
				if (Prev)
				{
					Prev->Next = This;
				}
				else
				{
					Channels.Front = This;
				}
				continue;
			}
			Prev = This;
			This = This->Next;
		}
	}
	catch (...)
	{
		SDL_UnlockAudio();
		throw;
	}
	SDL_UnlockAudio();
}

void Sound::SDLSystem::Update()
{
}

Sound::SDLSystem::~SDLSystem()
{
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
