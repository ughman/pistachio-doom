#ifndef Exception_hpp
#define Exception_hpp

#include "String.hpp"

class Exception
{
public:
	Exception() {}

	virtual const char *Message() const = 0;

	virtual ~Exception() {}
};

class StrException : public Exception
{
private:
	String Msg;
public:
	StrException(const String &Msg) : Msg(Msg) {}

	virtual const char *Message() const { return Msg.Value; }

	virtual ~StrException() {}
};

#endif
