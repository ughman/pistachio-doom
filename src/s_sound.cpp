#include "Memory.hpp"
#include "Sound/System.hpp"
#include "Sound/SDLSystem.hpp"
#include "Sound/PCMStream.hpp"
#include "Sound/LoopingStream.hpp"

extern "C"
{
#include "i_system.h"
#include "i_sound.h"
#include "sounds.h"
#include "s_sound.h"

#include "z_zone.h"
#include "m_random.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "doomstat.h"
}

int snd_MusicVolume;
int snd_SfxVolume;

Sound::System *sound = 0;

extern "C" void S_Init(int sfxVolume,int musicVolume)
{
	sound = new Sound::SDLSystem();
}

extern "C" void S_Start()
{
	//sound->StopAll();
	switch (gamemode)
	{
	case shareware:
	case registered:
	case retail:
		break;
	case commercial:
		S_ChangeMusic(mus_runnin + gamemap - 1,true);
		break;
	}
}

extern "C" void S_StartSound(void *origin,int id)
{
	mobj_t *mo = (mobj_t *)origin;
	sfxinfo_t *sfx = S_sfx + id;
	Sound::Stream *S = (Sound::Stream *)sfx->data;
	if (origin)
	{
		sound->Stop(origin);
	}
	if (sfx->lumpnum == 0)
	{
		char Name[9];
		strcpy(Name,"ds");
		strcat(Name,sfx->name);
		sfx->lumpnum = W_GetNumForName(Name);
	}
	if (!S)
	{
		unsigned char *Data;
		size_t Length = W_LumpLength(sfx->lumpnum);
		using (Data = new unsigned char [Length])
		{
			W_ReadLump(sfx->lumpnum,Data);
			Memory::Move(Data,Data + 8,Length - 8);
			Data = (unsigned char *)Memory::Reallocate(Data,Length - 8);
			S = new Sound::PCMStream(0,Data,Length - 8);
		}
		end_using_array(Data);
		sfx->data = S;
	}
	sound->Play(S,origin);
}

extern "C" void S_StopSound(void *origin)
{
	sound->Stop(origin);
}

extern "C" void S_PauseSound()
{
	// Music, not sfx!
}

extern "C" void S_ResumeSound()
{
	// Music, not sfx!
}

extern "C" void S_UpdateSounds(void *listener)
{
	mobj_t *mo = (mobj_t *)listener;
}

extern "C" void S_SetMusicVolume(int volume)
{
	snd_MusicVolume = volume;
}

extern "C" void S_SetSfxVolume(int volume)
{
	snd_SfxVolume = volume;
}

extern "C" void S_StartMusic(int id)
{
	S_ChangeMusic(id,false);
}

extern "C" void S_ChangeMusic(int id,int looping)
{
	return;
	musicinfo_t *music = S_music + id;
	Sound::Stream *S = (Sound::Stream *)music->data;
	sound->Stop(sound);
	if (music->lumpnum == 0)
	{
		char Name[9];
		strcpy(Name,"d_");
		strcat(Name,music->name);
		music->lumpnum = W_GetNumForName(Name);
	}
	if (!S)
	{
		unsigned char *Data;
		size_t Length = W_LumpLength(music->lumpnum);
		using (Data = new unsigned char [Length])
		{
			W_ReadLump(music->lumpnum,Data);
			S = new Sound::PCMStream(0,Data,Length);
		}
		end_using_array(Data);
		using (S)
		{
			if (looping)
			{
				S = new Sound::LoopingStream(0,S);
			}
		}
		end_using(S);
		music->data = S;
	}
	sound->Play(S,sound);
}
