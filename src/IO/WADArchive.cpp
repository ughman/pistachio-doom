#include "../String.hpp"
#include "../Memory.hpp"
#include "WADArchive.hpp"

IO::Archive *IO::WADArchive::Load(const char *Filename)
{
	String StrFilename(Filename);
	if (StrFilename.Length > 4 && !String::ICompare(Filename + StrFilename.Length - 4,".wad"))
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
	using (LumpTable = new unsigned char [LumpCount * 16])
	{
		F.Read(LumpTableOffset,LumpTable,LumpCount * 16);
		for (size_t i = 0;i < LumpCount;i++)
		{
			String LumpName((char *)LumpTable + (i * 16 + 8),8);
			try
			{
				Lumps.Add(LumpName,i);
			}
			catch (HashTable<String,unsigned int>::DuplicateKeyException &Error)
			{
				Lumps.Remove(LumpName);
				Lumps.Add(LumpName,i);
			}
		}
	}
	end_using_array(LumpTable);
}

unsigned int IO::WADArchive::Find(const char *Name)
{
	String LumpName(Name);
	LumpName.Uppercase();
	try
	{
		return Lumps.Find(LumpName)->Value;
	}
	catch (HashTable<String,unsigned int>::KeyNotFoundException &Error)
	{
		throw IO::Archive::EntryNotFoundException();
	}
}

String IO::WADArchive::ReverseFind(unsigned int Id)
{
	if (Id >= LumpCount)
	{
		throw StrException("Lump ID out of bounds.");
	}
	else
	{
		unsigned char *Entry;
		Entry = LumpTable + Id * 16;
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
		unsigned char *Entry;
		Entry = LumpTable + Id * 16;
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
		unsigned char *Entry;
		Entry = LumpTable + Id * 16;
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
	delete [] LumpTable;
}
