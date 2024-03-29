#include <cstdio>

#include "Legacy.hpp"
#include "Exception.hpp"
#include "Core/Init.hpp"

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
		Core::Init();
		Core::System Sys(M_CheckParm("-fullscreen"));
		LEGACY_ENTER(&Sys);
		D_DoomMain();
		LEGACY_LEAVE();
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
