#ifndef String_hpp
#define String_hpp

#include <cstdarg>

class String
{
public:
	static String Format(const char *Format,...);
	static String VFormat(const char *Format,va_list Args);

	char *Value;
	size_t Length;

	String();
	String(const String &Source);
	String(const char *Source);
	String(const char *Source,size_t Length);

	String &operator=(const String &Other);

	bool operator==(const String &Other) const;

	~String();
};

#endif
