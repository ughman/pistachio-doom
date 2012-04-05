#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include "String.hpp"
#include "Memory.hpp"

#ifdef WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

String String::Format(const char *Format,...)
{
	String Result;
	va_list Args;
	va_start(Args,Format);
	try
	{
		Result = VFormat(Format,Args);
	}
	catch (...)
	{
		va_end(Args);
		throw;
	}
	va_end(Args);
	return Result;
}

String String::VFormat(const char *Format,va_list Args)
{
	return String("TODO :: String formatting");
}

String::String()
{
	Length = 0;
	using (Value = new char [1])
	{
		Value[0] = 0;
	}
	end_using_array(Value);
}

String::String(const String &Source)
{
	Length = Source.Length;
	using (Value = new char [Length + 1])
	{
		Memory::Copy(Value,Source.Value,Length + 1);
	}
	end_using_array(Value);
}

String::String(const char *Source)
{
	Length = strlen(Source);
	using (Value = new char [Length + 1])
	{
		Memory::Copy(Value,Source,Length + 1);
	}
	end_using_array(Value);
}

String::String(const char *Source,size_t MaxLength)
{
	Length = strnlen(Source,MaxLength);
	using (Value = new char [Length + 1])
	{
		Memory::Copy(Value,Source,Length);
		Value[Length] = 0;
	}
	end_using_array(Value);
}

String &String::operator=(const String &Other)
{
	Value = (char *)Memory::Reallocate(Value,Other.Length + 1);
	Length = Other.Length;
	Memory::Copy(Value,Other.Value,Length + 1);
	return *this;
}

bool String::operator==(const String &Other) const
{
	if (Length != Other.Length) return false;
	return !Memory::Compare(Value,Other.Value,Length);
}

void String::Uppercase()
{
	for (size_t i = 0;i < Length;i++)
	{
		if (Value[i] >= 'a' && Value[i] <= 'z')
		{
			Value[i] -= 'a';
			Value[i] += 'A';
		}
	}
}

void String::Lowercase()
{
	for (size_t i = 0;i < Length;i++)
	{
		if (Value[i] >= 'A' && Value[i] <= 'Z')
		{
			Value[i] -= 'A';
			Value[i] += 'a';
		}
	}
}

String::~String()
{
	delete [] Value;
}

unsigned int String::Hash() const
{
	unsigned int X = 0;
	for (size_t i = 0;i < Length;i++)
	{
		unsigned char Low = X;
		X >>= CHAR_BIT;
		X |= Low << (CHAR_BIT * (sizeof(int) - 1));
		X ^= Value[i];
	}
	return X;
}

int String::Compare(const char *Left,const char *Right)
{
	return strcmp(Left,Right);
}

int String::ICompare(const char *Left,const char *Right)
{
	return strcasecmp(Left,Right);
}

int String::NCompare(const char *Left,const char *Right,size_t Length)
{
	return strncmp(Left,Right,Length);
}

int String::NICompare(const char *Left,const char *Right,size_t Length)
{
	return strncasecmp(Left,Right,Length);
}
