#ifndef Core_System_hpp
#define Core_System_hpp

#include "../Sound/System.hpp"
#include "../Video/System.hpp"
#include "../IO/MultiArchive.hpp"

namespace Core
{
	class System
	{
	private:
		System(const System &);
		System &operator=(const System &);
	public:
		Sound::System *Sound;
		Video::System *Video;
		IO::MultiArchive *Data;

		System(bool Fullscreen);

		~System();
	};
}

#endif
