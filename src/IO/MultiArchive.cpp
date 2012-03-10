#include "MultiArchive.hpp"

IO::MultiArchive::MultiArchive()
{
}

unsigned int IO::MultiArchive::Find(const char *Name)
{
	unsigned int Offset = Count();
	// Search back to front, so newer archives override older ones
	for (PtrLink <Archive> *it = Archives.Back;it;it = it->Prev)
	{
		Offset -= it->Value->Count();
		try
		{
			return Offset + it->Value->Find(Name);
		}
		catch (IO::Archive::EntryNotFoundException &Error)
		{
			// Skip this archive, try the next one
		}
	}
	// Not found in any of the archives, throw exception
	throw IO::Archive::EntryNotFoundException();
}

String IO::MultiArchive::ReverseFind(unsigned int Id)
{
	for (PtrLink <Archive> *it = Archives.Front;it;it = it->Next)
	{
		if (Id >= it->Value->Count())
		{
			Id -= it->Value->Count();
		}
		else
		{
			return it->Value->ReverseFind(Id);
		}
	}
	throw StrException("No such entry ID.");
}

void IO::MultiArchive::Read(unsigned int Id,void *Destination)
{
	for (PtrLink <Archive> *it = Archives.Front;it;it = it->Next)
	{
		if (Id >= it->Value->Count())
		{
			Id -= it->Value->Count();
		}
		else
		{
			it->Value->Read(Id,Destination);
			return;
		}
	}
	throw StrException("No such entry ID.");
}

size_t IO::MultiArchive::Length(unsigned int Id)
{
	for (PtrLink <Archive> *it = Archives.Front;it;it = it->Next)
	{
		if (Id >= it->Value->Count())
		{
			Id -= it->Value->Count();
		}
		else
		{
			return it->Value->Length(Id);
		}
	}
	throw StrException("No such entry ID.");
}

unsigned int IO::MultiArchive::Count()
{
	unsigned int Result = 0;
	for (PtrLink <Archive> *it = Archives.Front;it;it = it->Next)
	{
		Result += it->Value->Count();
	}
	return Result;
}

void IO::MultiArchive::Add(IO::Archive *Archive)
{
	Archives.Add(Archive);
}

IO::MultiArchive::~MultiArchive()
{
}
