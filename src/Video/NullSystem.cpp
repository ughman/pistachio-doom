#include "NullSystem.hpp"

Video::NullSystem::NullSystem()
{
}

void Video::NullSystem::Write(unsigned char *Buffer)
{
}

void Video::NullSystem::SetPalette(unsigned char *Palette)
{
}

bool Video::NullSystem::Update(Event *E)
{
	return false;
}

Video::NullSystem::~NullSystem()
{
}
