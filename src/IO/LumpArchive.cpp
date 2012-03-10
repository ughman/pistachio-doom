#include "../String.hpp"
#include "../Memory.hpp"
#include "LumpArchive.hpp"

IO::Archive *IO::LumpArchive::Load(const char *Filename)
{
	String StrFilename(Filename);
	if (StrFilename.Length > 4 && !Memory::Compare(Filename + StrFilename.Length - 4,".lmp",4))
	{
		return new IO::LumpArchive(Filename);
	}
	else
	{
		return 0;
	}
}

IO::LumpArchive::LumpArchive(const char *Filename) :
F(Filename,false) ,
Filename(Filename)
{
}

unsigned int IO::LumpArchive::Find(const char *Name)
{
	if ((Filename.Length > 4) && !String::NICompare(Filename.Value,Name,Filename.Length - 4))
	{
		return 0;
	}
	else
	{
		throw IO::Archive::EntryNotFoundException();
	}
}

String IO::LumpArchive::ReverseFind(unsigned int Id)
{
	if (Id > 0)
	{
		throw StrException("No such entry ID.");
	}
	return "";
}

void IO::LumpArchive::Read(unsigned int Id,void *Destination)
{
	if (Id > 0)
	{
		throw StrException("No such entry ID.");
	}
	F.Read(0,Destination,F.Length());
}

size_t IO::LumpArchive::Length(unsigned int Id)
{
	if (Id > 0)
	{
		throw StrException("No such entry ID.");
	}
	return F.Length();
}

unsigned int IO::LumpArchive::Count()
{
	return 1;
}

IO::LumpArchive::~LumpArchive()
{
}
