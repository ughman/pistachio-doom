#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include "String.hpp"
#include "Memory.hpp"

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

String::String(const char *Source,size_t Length)
{
	Length = strnlen(Source,Length);
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

String::~String()
{
	delete [] Value;
}
