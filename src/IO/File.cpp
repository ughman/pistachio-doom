#include "../Exception.hpp"
#include "File.hpp"

IO::File::File(const char *Filename,bool Writable) :
Writable(Writable)
{
	if (!(F = fopen(Filename,Writable ? "rb+" : "rb")))
	{
		throw StrException("Failed to open file.");
	}
}

void IO::File::Read(unsigned long Position,void *Buffer,size_t Size)
{
	if (Size == 0) return;
	if (fseek(F,Position,SEEK_SET))
	{
		throw StrException("Failed to seek file.");
	}
	if (fread(Buffer,Size,1,F) != 1)
	{
		throw StrException("Failed to read file.");
	}
}

void IO::File::Write(unsigned long Position,const void *Buffer,size_t Size)
{
	if (Size == 0) return;
	if (!Writable)
	{
		throw StrException("Tried to write non-writable file.");
	}
	if (fseek(F,Position,SEEK_SET))
	{
		throw StrException("Failed to seek file.");
	}
	if (fwrite(Buffer,Size,1,F) != 1)
	{
		throw StrException("Failed to write file.");
	}
}

size_t IO::File::Length()
{
	if (fseek(F,0,SEEK_END))
	{
		throw StrException("Failed to seek file.");
	}
	long Result = ftell(F);
	if (Result == -1)
	{
		throw StrException("Failed to tell file.");
	}
	return Result;
}

IO::File::~File()
{
	fclose(F);
}
