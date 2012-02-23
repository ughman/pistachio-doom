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
	((Sound::SDLSystem *)Sys)->Callback(Output,OutLength);
}

void Sound::SDLSystem::Callback(unsigned char *Output,int OutLength)
{
	Memory::Set(Output,0,OutLength);
	size_t ChannelCount = Channels.Count();
	unsigned char *X = new unsigned char [OutLength];
	try
	{
		PtrLink <Sound::Stream> *Prev = 0;
		PtrLink <Sound::Stream> *This = Channels.Front;
		while (This)
		{
			size_t Played = This->Value->Play(X,OutLength);
			if (Played > 0)
			{
				SDL_MixAudio(Output,X,Played,SDL_MIX_MAXVOLUME);
			}
			else
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
		Spec.format = AUDIO_U8;
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
