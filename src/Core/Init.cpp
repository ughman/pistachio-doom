#include "Init.hpp"
#include "../IO/Archive.hpp"
#include "../IO/LumpArchive.hpp"
#include "../IO/WADArchive.hpp"
#include "../Sound/Stream.hpp"
#include "../Sound/PCMStream.hpp"
#include "../Sound/MUSStream.hpp"
#include "../Sound/MIDIStream.hpp"

void Core::Init()
{
	IO::Archive::Formats.Add(IO::LumpArchive::Load);
	IO::Archive::Formats.Add(IO::WADArchive::Load);
	Sound::Stream::Formats.Add(Sound::PCMStream::LoadDoomWave);
	Sound::Stream::Formats.Add(Sound::MUSStream::Load);
	Sound::Stream::Formats.Add(Sound::MIDIStream::Load);
}
