#include "../Queue.hpp"
#include "../Memory.hpp"
#include "../Exception.hpp"
#include "GLFWSystem.hpp"

static Queue <Video::Event> EventQueue;
static Video::GLFWSystem *VideoSystem = 0;

static Video::KeySym TranslateKey(int Key)
{
	if (Key >= 'A' && Key <= 'Z')
	{
		return (Video::KeySym)((Key - 'A') + 'a');
	}
	else switch (Key)
	{
	case GLFW_KEY_BACKSPACE:
		return Video::EVK_BACKSPACE;
	case GLFW_KEY_PAUSE:
		return Video::EVK_PAUSE;
	case GLFW_KEY_ESC:
		return Video::EVK_ESCAPE;
	case GLFW_KEY_ENTER:
		return Video::EVK_ENTER;
	case GLFW_KEY_TAB:
		return Video::EVK_TAB;
	case GLFW_KEY_RSHIFT:
	case GLFW_KEY_LSHIFT:
		return Video::EVK_SHIFT;
	case GLFW_KEY_RCTRL:
	case GLFW_KEY_LCTRL:
		return Video::EVK_CTRL;
	case GLFW_KEY_RALT:
	case GLFW_KEY_LALT:
		return Video::EVK_ALT;
	case GLFW_KEY_UP:
		return Video::EVK_UP;
	case GLFW_KEY_DOWN:
		return Video::EVK_DOWN;
	case GLFW_KEY_LEFT:
		return Video::EVK_LEFT;
	case GLFW_KEY_RIGHT:
		return Video::EVK_RIGHT;
#define XLATF(id) case GLFW_KEY_F##id: return Video::EVK_F##id
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

static void GLFWCALL KeyCallback(int Key,int Action)
{
	Video::Event Event;
	if (Action == GLFW_PRESS)
	{
		Event.Type = Video::EVT_KEYDOWN;
	}
	else
	{
		Event.Type = Video::EVT_KEYUP;
	}
	Event.Key.Key = TranslateKey(Key);
	EventQueue.Enqueue(Event);
}

static void MouseCallback()
{
	int x,y;
	glfwGetMousePos(&x,&y);
	Video::Event Event;
	Event.Type = Video::EVT_MOUSE;
	Event.Mouse.Buttons = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) | (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) << 2);
	for (int i = 0;i <= GLFW_MOUSE_BUTTON_LAST;i++)
	{
		Event.Mouse.Buttons |= (glfwGetMouseButton(i) == GLFW_PRESS) << i;
	}
	Event.Mouse.XOffset = x * 4;
	Event.Mouse.YOffset = y * -4;
	EventQueue.Enqueue(Event);
}

static void GLFWCALL MouseButtonCallback(int Button,int Action)
{
	MouseCallback();
}

static void GLFWCALL MouseMoveCallback(int x,int y)
{
	MouseCallback();
}

static int GLFWCALL WindowCloseCallback()
{
	Video::Event Event;
	Event.Type = Video::EVT_QUIT;
	EventQueue.Enqueue(Event);
	return GL_FALSE;
}

static void GLFWCALL WindowResizeCallback(int Width,int Height)
{
	VideoSystem->Width = Width;
	VideoSystem->Height = Height;
	glViewport(0,0,Width,Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,320,200,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Video::GLFWSystem::GLFWSystem(int Width,int Height,bool Fullscreen)
{
	if (VideoSystem)
	{
		throw StrException("Video::GLFWSystem already in use.");
	}
	if (glfwInit() != GL_TRUE)
	{
		throw StrException("GLFW initialization failed.");
	}
	try
	{
		if (glfwOpenWindow(Width,Height,8,8,8,0,0,0,Fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW) != GL_TRUE)
		{
			throw StrException("SDL video mode setting failed.");
		}
		VideoSystem = this;
		glfwDisable(GLFW_MOUSE_CURSOR);
		WindowResizeCallback(Width,Height);
		glfwSetKeyCallback(KeyCallback);
		glfwSetMousePosCallback(MouseMoveCallback);
		glfwSetMouseButtonCallback(MouseButtonCallback);
		glfwSetWindowCloseCallback(WindowCloseCallback);
		glfwSetWindowSizeCallback(WindowResizeCallback);
	}
	catch (...)
	{
		glfwTerminate();
		throw;
	}
}

void Video::GLFWSystem::Read(unsigned char *Buffer)
{
	throw StrException("Video::GLFWSystem::Read not implemented.");
}

void Video::GLFWSystem::Write(unsigned char *Buffer,bool Update)
{
	static unsigned char Image[512 * 256 * 3];
	for (size_t x = 0;x < 320;x++)
	for (size_t y = 0;y < 200;y++)
	{
		unsigned char Color = Buffer[x + y * 320];
		unsigned char Red   = Palette[0 + Color * 3];
		unsigned char Green = Palette[1 + Color * 3];
		unsigned char Blue  = Palette[2 + Color * 3];
		Image[0 + (x + y * 512) * 3] = Red;
		Image[1 + (x + y * 512) * 3] = Green;
		Image[2 + (x + y * 512) * 3] = Blue;
	}
	double tx2 = 320 / 512.0;
	double ty2 = 200 / 256.0;
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,256,0,GL_RGB,GL_UNSIGNED_BYTE,Image);
	glBegin(GL_QUADS);
		glColor3ub(255,255,255);
		glTexCoord2d(0,0);
		glVertex2d(0,0);
		glTexCoord2d(tx2,0);
		glVertex2d(320,0);
		glTexCoord2d(tx2,ty2);
		glVertex2d(320,200);
		glTexCoord2d(0,ty2);
		glVertex2d(0,200);
	glEnd();
	if (Update)
	{
		glfwSwapBuffers();
	}
}

void Video::GLFWSystem::SetPalette(unsigned char *Palette)
{
	Memory::Copy(this->Palette,Palette,256 * 3);
}

bool Video::GLFWSystem::Update(Video::Event *E)
{
	glfwPollEvents();
	glfwSetMousePos(0,0);
	try
	{
		*E = EventQueue.Dequeue();
		return true;
	}
	catch (Queue<Video::Event>::EmptyException &Err)
	{
		return false;
	}
}

Video::GLFWSystem::~GLFWSystem()
{
	EventQueue.Clear();
	glfwTerminate();
	VideoSystem = 0;
}
