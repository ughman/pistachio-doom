#ifndef Video_GLFWSystem_hpp
#define Video_GLFWSystem_hpp

#include <GL/glfw.h>

#include "System.hpp"

namespace Video
{
	class GLFWSystem : public System
	{
	private:
		GLFWSystem(const GLFWSystem &);
		GLFWSystem &operator=(const GLFWSystem &);

		unsigned char Palette[256 * 3];
	public:
		int Width;
		int Height;

		explicit GLFWSystem(int Width,int Height,bool Fullscreen);

		virtual void Write(unsigned char *Buffer);
		virtual void SetPalette(unsigned char *Palette);

		virtual bool Update(Event *E);

		virtual ~GLFWSystem();
	};
}

#endif
