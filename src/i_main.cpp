#include <cstdio>

#include "Exception.hpp"

extern "C"
{
#include "m_argv.h"
#include "d_main.h"
}

int main(int argc,char **argv)
{
	myargc = argc;
	myargv = argv;

#ifdef NDEBUG
	try
#endif
	{
		D_DoomMain();
	}
#ifdef NDEBUG
	catch (Exception &Error)
	{
		fprintf(stderr,"Unhandled Exception: %s\n",Error.Message());
		fflush(stderr);
		return 0;
	}
#endif
	return 0;
}
