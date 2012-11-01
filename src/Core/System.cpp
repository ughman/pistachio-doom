#include "../Exception.hpp"
#include "System.hpp"
#include "../Sound/SDLSystem.hpp"
#include "../Sound/NullSystem.hpp"

Core::System::System()
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
}

Core::System::~System()
{
	delete Sound;
}
