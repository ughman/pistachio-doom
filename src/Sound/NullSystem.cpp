#include "NullSystem.hpp"

Sound::NullSystem::NullSystem(float SoundVolume,float MusicVolume) :
Sound::System(SoundVolume,MusicVolume)
{
}

void Sound::NullSystem::PlaySound(Stream *S,void *UserData)
{
}

void Sound::NullSystem::PlayMusic(Stream *S,void *UserData)
{
}

void Sound::NullSystem::StopSound(void *UserData)
{
}

void Sound::NullSystem::StopMusic(void *UserData)
{
}

void Sound::NullSystem::StopAll()
{
}

void Sound::NullSystem::Update()
{
}

Sound::NullSystem::~NullSystem()
{
}
