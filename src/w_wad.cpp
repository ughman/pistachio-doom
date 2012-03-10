#include "IO/Archive.hpp"
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

IO::MultiArchive *archive = 0;

// W_AddFile

extern "C" void W_Reload()
{
	// TODO
}

extern "C" void W_InitMultipleFiles(char **Filenames)
{
	IO::Archive::Formats.Add(IO::LumpArchive::Load);
	IO::Archive::Formats.Add(IO::WADArchive::Load);
	archive = new IO::MultiArchive();
	while (*Filenames && **Filenames)
	{
		using (IO::Archive *A = IO::Archive::Load(*Filenames))
		{
			archive->Add(A);
		}
		end_using(A);
		Filenames++;
	}
}

// W_InitFile

// W_NumLumps

extern "C" int W_CheckNumForName(char *Name)
{
	try
	{
		return archive->Find(Name);
	}
	catch (IO::Archive::EntryNotFoundException &Error)
	{
		return -1;
	}
}

extern "C" int W_GetNumForName(char *Name)
{
	return archive->Find(Name);
}

extern "C" void W_GetNameForNum(int Lump,char *Name)
{
	String Result = archive->ReverseFind(Lump);
	strncpy(Name,Result.Value,8);
}

extern "C" int W_LumpLength(int Lump)
{
	return archive->Length(Lump);
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
		archive->Read(Lump,Destination);
	}
}

extern "C" void *W_CacheLumpNum(int Lump,int Tag)
{
	if (!lumpcache)
	{
		lumpcache = new void * [archive->Count()];
		memset(lumpcache,0,archive->Count() * sizeof(void *));
	}
	if (!lumpcache[Lump])
	{
		lumpcache[Lump] = Z_Malloc(W_LumpLength(Lump),Tag,lumpcache + Lump);
		archive->Read(Lump,lumpcache[Lump]);
	}
	return lumpcache[Lump];
}

extern "C" void *W_CacheLumpName(char *Name,int Tag)
{
	return W_CacheLumpNum(W_GetNumForName(Name),Tag);
}

// W_Profile
