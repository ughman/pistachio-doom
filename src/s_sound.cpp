#include "Math.hpp"
#include "Vector.hpp"
#include "Memory.hpp"
#include "Legacy.hpp"
#include "Sound/System.hpp"
#include "Sound/PCMStream.hpp"
#include "Sound/MUSStream.hpp"
#include "Sound/MIDIStream.hpp"
#include "Sound/LoopingStream.hpp"
#include "Sound/Synth/Instrument.hpp"

extern "C"
{
#include "i_system.h"
#include "sounds.h"
#include "s_sound.h"

#include "z_zone.h"
#include "m_random.h"
#include "m_argv.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "doomstat.h"
}

int snd_MusicVolume;
int snd_SfxVolume;

Vector <Sound::Synth::Instrument> genmidi(175);

static void S_LoadGMOperator(unsigned char *Data,Sound::Synth::Operator *O)
{
	O->Tremolo = Data[0] & 0x80;
	O->Vibrato = Data[0] & 0x40;
	O->DoSustain = Data[0] & 0x20;
	O->KSR = Data[0] & 0x10;
	O->Multiplier = Data[0] & 0xF;
	if (O->Multiplier == 0)
	{
		O->Multiplier = 0.5f;
	}
	O->Attack = (Data[1] >> 4) / 15.f;
	O->Decay = (Data[1] & 0xF) / 15.f;
	O->Sustain = (Data[2] >> 4) / 15.f;
	O->Release = (Data[2] & 0xF) / 15.f;
	O->WaveformType = (Sound::Synth::WaveformType)Data[3];
}

static void S_LoadGMVoice(unsigned char *Data,Sound::Synth::Operator *Modulator,Sound::Synth::Operator *Carrier,bool *Connection,float *Feedback,short *Offset)
{
	S_LoadGMOperator(Data,Modulator);
	switch ((Data[6] & 0xE) >> 1)
	{
	case 0:
		*Feedback = 0;
		break;
	case 1:
		*Feedback = .125;
		break;
	case 2:
		*Feedback = .25;
		break;
	case 3:
		*Feedback = .5;
		break;
	case 4:
		*Feedback = 1;
		break;
	case 5:
		*Feedback = 2;
		break;
	case 6:
		*Feedback = 4;
		break;
	case 7:
		*Feedback = 8;
		break;
	}
	S_LoadGMOperator(Data + 7,Carrier);
	*Connection = Data[6] & 0x1;
	*Offset = Data[14];
	*Offset |= Data[15] << 8;
}

static void S_LoadGMInstrument(unsigned char *Data,Sound::Synth::Instrument *I)
{
	I->FixedPitch = Data[0] & 0x1;
	I->DoubleVoice = Data[0] & 0x4;
	I->Tuning[0] = 0;
	I->Tuning[1] = (signed short)Data[2] - 128;
	I->Note = Data[3];
	S_LoadGMVoice(Data + 4,&I->Modulator[0],&I->Carrier[0],&I->Connection[0],&I->Feedback[0],&I->Offset[0]);
	S_LoadGMVoice(Data + 20,&I->Modulator[1],&I->Carrier[1],&I->Connection[1],&I->Feedback[1],&I->Offset[1]);
}

extern "C" void S_Init(int sfxVolume,int musicVolume)
{
	int genmidinum = W_GetNumForName("GENMIDI");
	unsigned char *Data = new unsigned char [W_LumpLength(genmidinum)];
	try
	{
		W_ReadLump(genmidinum,Data);
		for (size_t i = 0;i < 175;i++)
		{
			S_LoadGMInstrument(Data + 8 + 36 * i,genmidi.Array + i);
		}
	}
	catch (...)
	{
		delete [] Data;
		throw;
	}
	delete [] Data;
}

extern "C" void S_Start()
{
	Pistachio->Sound->StopAll();
	switch (gamemode)
	{
	case shareware:
	case registered:
	case retail:
		switch (gameepisode)
		{
		case 1:
			S_ChangeMusic(mus_e1m1 + gamemap - 1,true);
			break;
		case 2:
			S_ChangeMusic(mus_e2m1 + gamemap - 1,true);
			break;
		case 3:
			S_ChangeMusic(mus_e3m1 + gamemap - 1,true);
			break;
		case 4:
			switch (gamemap)
			{
			case 1:
				S_ChangeMusic(mus_e3m4,true);
				break;
			case 2:
				S_ChangeMusic(mus_e3m2,true);
				break;
			case 3:
				S_ChangeMusic(mus_e3m3,true);
				break;
			case 4:
				S_ChangeMusic(mus_e1m5,true);
				break;
			case 5:
				S_ChangeMusic(mus_e2m7,true);
				break;
			case 6:
				S_ChangeMusic(mus_e2m4,true);
				break;
			case 7:
				S_ChangeMusic(mus_e2m6,true);
				break;
			case 8:
				S_ChangeMusic(mus_e2m5,true);
				break;
			case 9:
				S_ChangeMusic(mus_e1m9,true);
				break;
			}
			break;
		}
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
		Pistachio->Sound->StopSound(origin);
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
			S = Sound::Stream::Load(Data,Length,genmidi);
		}
		end_using_array(Data);
		delete [] Data;
		sfx->data = S;
	}
	Pistachio->Sound->PlaySound(S,origin);
}

extern "C" void S_StopSound(void *origin)
{
	Pistachio->Sound->StopSound(origin);
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
	Pistachio->Sound->MusicVolume = volume / 15.f;
}

extern "C" void S_SetSfxVolume(int volume)
{
	snd_SfxVolume = volume;
	Pistachio->Sound->SoundVolume = volume / 15.f;
}

extern "C" void S_StartMusic(int id)
{
	S_ChangeMusic(id,false);
}

extern "C" void S_ChangeMusic(int id,int looping)
{
	musicinfo_t *music = S_music + id;
	Sound::Stream *S = (Sound::Stream *)music->data;
	Pistachio->Sound->StopMusic(0);
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
			unsigned short InstrumentCount;
			W_ReadLump(music->lumpnum,Data);
			S = Sound::Stream::Load(Data,Length,genmidi);
		}
		end_using_array(Data);
		delete [] Data;
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
	Pistachio->Sound->PlayMusic(S,0);
}
