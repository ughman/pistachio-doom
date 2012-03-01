#include "Memory.hpp"
#include "Video/System.hpp"
#include "Video/SDLSystem.hpp"

extern "C"
{
#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"
}

Video::System *video = 0;

extern "C" void I_ShutdownGraphics()
{
	delete video;
	video = 0;
}

extern "C" void I_StartFrame()
{
}

// I_GetEvent()

extern "C" void I_StartTic()
{
	static unsigned int buttons = 0;
	Video::Event e;
	event_t de;
	while (video->Update(&e))
	{
		switch (e.Type)
		{
		case Video::EVT_KEYDOWN:
			de.type = ev_keydown;
			de.data1 = e.Key.Key;
			D_PostEvent(&de);
			break;
		case Video::EVT_KEYUP:
			de.type = ev_keyup;
			de.data1 = e.Key.Key;
			D_PostEvent(&de);
			break;
		case Video::EVT_MOUSE:
			de.type = ev_mouse;
			de.data1 = e.Mouse.Buttons;
			de.data2 = e.Mouse.XOffset;
			de.data3 = e.Mouse.YOffset;
			D_PostEvent(&de);
			break;
		}
	}
}

extern "C" void I_UpdateNoBlit()
{
	video->Write(screens[0],false);
}

extern "C" void I_FinishUpdate()
{
	video->Write(screens[0],true);
}

extern "C" void I_ReadScreen(byte *scr)
{
	Memory::Copy(scr,screens[0],SCREENWIDTH * SCREENHEIGHT);
}

extern "C" void I_SetPalette(byte *palette)
{
	video->SetPalette(palette);
}

extern "C" void I_InitGraphics()
{
	video = new Video::SDLSystem(SCREENWIDTH,SCREENHEIGHT,M_CheckParm("-fullscreen"));
}
