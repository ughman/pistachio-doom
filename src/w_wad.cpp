#include "Legacy.hpp"
#include "IO/MultiArchive.hpp"
#include "IO/LumpArchive.hpp"
#include "IO/WADArchive.hpp"

extern "C"
{
#include "doomtype.h"
#include "m_swap.h"
#include "i_system.h"
#include "z_zone.h"

#include "w_wad.h"
}

// W_AddFile

extern "C" void W_Reload()
{
	// TODO
}

extern "C" void W_InitMultipleFiles(char **Filenames)
{
	while (*Filenames && **Filenames)
	{
		using (IO::Archive *A = IO::Archive::Load(*Filenames))
		{
			Pistachio->Data->Add(A);
		}
		end_using(A);
		Filenames++;
	}
}

// W_InitFile

// W_NumLumps

extern "C" int W_CheckNumForName(char *Name)
{
	char RealName[9];
	RealName[8] = 0;
	strncpy(RealName,Name,8);
	try
	{
		return Pistachio->Data->Find(RealName);
	}
	catch (IO::Archive::EntryNotFoundException &Error)
	{
		return -1;
	}
}

extern "C" int W_GetNumForName(char *Name)
{
	char RealName[9];
	RealName[8] = 0;
	strncpy(RealName,Name,8);
	return Pistachio->Data->Find(RealName);
}

extern "C" void W_GetNameForNum(int Lump,char *Name)
{
	String Result = Pistachio->Data->ReverseFind(Lump);
	strncpy(Name,Result.Value,8);
}

extern "C" int W_LumpLength(int Lump)
{
	return Pistachio->Data->Length(Lump);
}

void **lumpcache = 0;

extern "C" void W_ReadLump(int Lump,void *Destination)
{
	if (lumpcache && lumpcache[Lump])
	{
		memcpy(Destination,lumpcache[Lump],W_LumpLength(Lump));
	}
	else
	{
		Pistachio->Data->Read(Lump,Destination);
	}
}

extern "C" void *W_CacheLumpNum(int Lump,int Tag)
{
	if (!lumpcache)
	{
		lumpcache = new void * [Pistachio->Data->Count()];
		memset(lumpcache,0,Pistachio->Data->Count() * sizeof(void *));
	}
	if (!lumpcache[Lump])
	{
		lumpcache[Lump] = Z_Malloc(W_LumpLength(Lump),Tag,lumpcache + Lump);
		Pistachio->Data->Read(Lump,lumpcache[Lump]);
	}
	return lumpcache[Lump];
}

extern "C" void *W_CacheLumpName(char *Name,int Tag)
{
	return W_CacheLumpNum(W_GetNumForName(Name),Tag);
}

// W_Profile
