#include <SDL.h>

#include "Exception.hpp"
#include "Memory.hpp"

extern "C"
{
#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

#include "i_system.h"
}

ticcmd_t ticcmd;

extern "C" void I_Tactile(int on,int off,int total)
{
	// TODO
}

extern "C" ticcmd_t *I_BaseTiccmd()
{
	return &ticcmd;
}

// I_GetHeapSize()

extern "C" byte *I_ZoneBase(int *size)
{
	*size = 16 * 1024 * 1024;
	return new byte [*size];
}

extern "C" int I_GetTime()
{
	return SDL_GetTicks() / (1000 / TICRATE);
}

extern "C" void I_Init()
{
	if (SDL_Init(0) == -1)
	{
		throw StrException("SDL initialization failed.");
	}
}

extern "C" void I_Quit()
{
	D_QuitNetGame ();
	I_ShutdownGraphics();
    M_SaveDefaults ();
	SDL_Quit();
    exit(0);
}

extern "C" void I_WaitVBL(int count)
{
	// TODO
}

// I_BeginRead()

// I_EndRead()

extern "C" byte *I_AllocLow(int length)
{
	byte *Result = new byte [length];
	Memory::Set(Result,0,length);
	return Result;
}

extern "C" void I_Error(char *error,...)
{
	va_list args;
	va_start(args,error);
	fprintf(stderr,"Error: ");
	vfprintf(stderr,error,args);
	fprintf(stderr,"\n");
	fflush(stderr);
	va_end(args);
	throw StrException("I_Error called.");
}
