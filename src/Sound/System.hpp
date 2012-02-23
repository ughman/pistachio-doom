#ifndef Sound_System_hpp
#define Sound_System_hpp

#include "Stream.hpp"

namespace Sound
{
	class System
	{
	private:
		System(const System &);
		System &operator=(const System &);
	public:
		System() {}

		virtual void Play(Stream *S,void *UserData) = 0;
		virtual void Stop(void *UserData) = 0;
		virtual void Update() = 0;

		virtual ~System() {}
	};
}

#endif
