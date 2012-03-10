#include "Archive.hpp"

List <IO::Archive::Format> IO::Archive::Formats;

IO::Archive *IO::Archive::Load(const char *Filename)
{
	for (Link <IO::Archive::Format> *it = IO::Archive::Formats.Front;it;it = it->Next)
	{
		if (IO::Archive *Result = it->Value(Filename))
		{
			return Result;
		}
	}
	throw StrException("Unknown archive format.");
}
