#include "../Exception.hpp"
#include "System.hpp"
#include "../Sound/SDLSystem.hpp"
#include "../Sound/NullSystem.hpp"
#include "../Video/GLFWSystem.hpp"
#include "../Video/NullSystem.hpp"

Core::System::System(bool Fullscreen)
{
	try
	{
		Sound = new Sound::SDLSystem(1.f,1.f);
	}
	catch (Exception &Error)
	{
		// TODO :: Report error
		Sound = new Sound::NullSystem(0,0);
	}
	using (Sound)
	{
		try
		{
			Video = new Video::GLFWSystem(800,600,Fullscreen);
		}
		catch (Exception &Error)
		{
			// TODO :: Report error
			Video = new Video::NullSystem();
		}
	}
	end_using(Sound);
}

Core::System::~System()
{
	delete Video;
	delete Sound;
}
