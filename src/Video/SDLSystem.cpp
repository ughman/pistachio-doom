#include "../Memory.hpp"
#include "../Exception.hpp"
#include "SDLSystem.hpp"

static Video::KeySym TranslateKey(SDLKey Key)
{
	switch (Key)
	{
	case SDLK_BACKSPACE:
		return Video::EVK_BACKSPACE;
	case SDLK_PAUSE:
		return Video::EVK_PAUSE;
	case SDLK_ESCAPE:
		return Video::EVK_ESCAPE;
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		return Video::EVK_SHIFT;
	case SDLK_RCTRL:
	case SDLK_LCTRL:
		return Video::EVK_CTRL;
	case SDLK_RALT:
	case SDLK_LALT:
	case SDLK_RMETA:
	case SDLK_LMETA:
		return Video::EVK_ALT;
	case SDLK_UP:
		return Video::EVK_UP;
	case SDLK_DOWN:
		return Video::EVK_DOWN;
	case SDLK_LEFT:
		return Video::EVK_LEFT;
	case SDLK_RIGHT:
		return Video::EVK_RIGHT;
#define XLATF(id) case SDLK_F##id: return Video::EVK_F##id
	XLATF(1);
	XLATF(2);
	XLATF(3);
	XLATF(4);
	XLATF(5);
	XLATF(6);
	XLATF(7);
	XLATF(8);
	XLATF(9);
	XLATF(10);
	XLATF(11);
	XLATF(12);
#undef XLATF
	default:
		return (Video::KeySym)Key;
	}
}

Video::SDLSystem::SDLSystem(int Width,int Height,bool Fullscreen) :
Width(Width),
Height(Height)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
	{
		throw StrException("SDL video initialization failed.");
	}
	try
	{
		Screen = SDL_SetVideoMode(Width,Height,8,Fullscreen ? (SDL_HWSURFACE | SDL_FULLSCREEN) : SDL_SWSURFACE);
		if (!Screen)
		{
			throw StrException("SDL video mode setting failed.");
		}
		SDL_ShowCursor(false);
	}
	catch (...)
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		throw;
	}
}

void Video::SDLSystem::Write(unsigned char *Buffer)
{
	if (SDL_LockSurface(Screen) == -1)
	{
		throw StrException("SDL surface lock failed.");
	}
	unsigned char *Pixels = (unsigned char *)Screen->pixels;
	short Pitch = Screen->pitch;
	for (int y = 0;y < Height;y++)
	{
		Memory::Copy(Pixels + y * Pitch,Buffer + y * Width,Width);
	}
	SDL_UnlockSurface(Screen);
	SDL_Flip(Screen);
}

void Video::SDLSystem::SetPalette(unsigned char *Palette)
{
	SDL_Color Colors[256];
	for (int i = 0;i < 256;i++)
	{
		Colors[i].r = Palette[i * 3 + 0];
		Colors[i].g = Palette[i * 3 + 1];
		Colors[i].b = Palette[i * 3 + 2];
	}
	if (!SDL_SetColors(Screen,Colors,0,256))
	{
		throw StrException("SDL palette change failed.");
	}
}

bool Video::SDLSystem::Update(Video::Event *E)
{
	SDL_Event SE;
	if (SDL_PollEvent(&SE))
	{
		switch (SE.type)
		{
		case SDL_KEYDOWN:
			E->Key.Type = EVT_KEYDOWN;
			E->Key.Key = TranslateKey(SE.key.keysym.sym);
			return true;
		case SDL_KEYUP:
			E->Key.Type = EVT_KEYUP;
			E->Key.Key = TranslateKey(SE.key.keysym.sym);
			return true;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			E->Mouse.Type = EVT_MOUSE;
			E->Mouse.Buttons = SDL_GetMouseState(&E->Mouse.XOffset,&E->Mouse.YOffset);
			E->Mouse.XOffset -= Width / 2;
			E->Mouse.YOffset -= Height / 2;
			E->Mouse.XOffset *= 4;
			E->Mouse.YOffset *= -4;
			SDL_WarpMouse(Width / 2,Height / 2);
			return true;
		case SDL_QUIT:
			E->Type = EVT_QUIT;
			return true;
		default:
			return Update(E);
		}
	}
	else
	{
		return false;
	}
}

Video::SDLSystem::~SDLSystem()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
