#include "../String.hpp"
#include "../Memory.hpp"
#include "WADArchive.hpp"

IO::Archive *IO::WADArchive::Load(const char *Filename)
{
	String StrFilename(Filename);
	if (StrFilename.Length > 4 && !Memory::Compare(Filename + StrFilename.Length - 4,".wad",4))
	{
		return new IO::WADArchive(Filename);
	}
	else
	{
		return 0;
	}
}

IO::WADArchive::WADArchive(const char *Filename) :
F(Filename,false)
{
	unsigned char Header[12];
	F.Read(0,Header,12);
	if (Memory::Compare(Header + 1,"WAD",3)) throw StrException("Invalid WAD header.");
	LumpCount  = Header[4];
	LumpCount |= Header[5] << 8;
	LumpCount |= Header[6] << 16;
	LumpCount |= Header[7] << 24;
	LumpTableOffset  = Header[8];
	LumpTableOffset |= Header[9]  << 8;
	LumpTableOffset |= Header[10] << 16;
	LumpTableOffset |= Header[11] << 24;
	if (LumpTableOffset + LumpCount * 16 > F.Length()) throw StrException("WAD lump table out of bounds.");
}

unsigned int IO::WADArchive::Find(const char *Name)
{
	unsigned char Entry[16];
	for (unsigned int i = 0;i < LumpCount;i++)
	{
		F.Read(LumpTableOffset + i * 16,Entry,16);
		if (!String::NICompare((char *)Entry + 8,Name,8))
		{
			return i;
		}
	}
	throw IO::Archive::EntryNotFoundException();
}

String IO::WADArchive::ReverseFind(unsigned int Id)
{
	if (Id >= LumpCount)
	{
		throw StrException("Lump ID out of bounds.");
	}
	else
	{
		unsigned char Entry[16];
		F.Read(LumpTableOffset + Id * 16,Entry,16);
		return String((char *)Entry + 8,8);
	}
}


void IO::WADArchive::Read(unsigned int Id,void *Destination)
{
	if (Id >= LumpCount)
	{
		throw StrException("Lump ID out of bounds.");
	}
	else
	{
		unsigned char Entry[16];
		F.Read(LumpTableOffset + Id * 16,Entry,16);
		unsigned long Offset;
		unsigned long Length;
		Offset  = Entry[0];
		Offset |= Entry[1] << 8;
		Offset |= Entry[2] << 16;
		Offset |= Entry[3] << 24;
		Length  = Entry[4];
		Length |= Entry[5] << 8;
		Length |= Entry[6] << 16;
		Length |= Entry[7] << 24;
		F.Read(Offset,Destination,Length);
	}
}

size_t IO::WADArchive::Length(unsigned int Id)
{
	if (Id >= LumpCount)
	{
		throw StrException("Lump ID out of bounds.");
	}
	else
	{
		unsigned char Entry[16];
		F.Read(LumpTableOffset + Id * 16,Entry,16);
		unsigned long Result;
		Result  = Entry[4];
		Result |= Entry[5] << 8;
		Result |= Entry[6] << 16;
		Result |= Entry[7] << 24;
		return Result;
	}
}

unsigned int IO::WADArchive::Count()
{
	return LumpCount;
}

IO::WADArchive::~WADArchive()
{
}
