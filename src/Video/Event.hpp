#ifndef Video_Event_hpp
#define Video_Event_hpp

namespace Video
{
	enum EventType
	{
		EVT_KEYDOWN,
		EVT_KEYUP,
		EVT_MOUSE,
		EVT_QUIT
	};

	enum KeySym
	{
		EVK_LEFT = 0xAC,
		EVK_UP,
		EVK_RIGHT,
		EVK_DOWN,
		EVK_ESCAPE = 27,
		EVK_ENTER = 13,
		EVK_TAB = 9,
		EVK_F1 = (0x80 + 0x3B),
		EVK_F2,
		EVK_F3,
		EVK_F4,
		EVK_F5,
		EVK_F6,
		EVK_F7,
		EVK_F8,
		EVK_F9,
		EVK_F10,
		EVK_F11 = (0x80 + 0x57),
		EVK_F12,
		EVK_BACKSPACE = 127,
		EVK_PAUSE = 255,
		EVK_EQUALS = 0x3D,
		EVK_MINUS = 0x2D,
		EVK_SHIFT = (0x80 + 0x36),
		EVK_CTRL = (0x80 + 0x1D),
		EVK_ALT = (0x80 + 0x38)
	};

	struct KeyEvent
	{
		EventType Type;
		KeySym Key;
	};

	struct MouseEvent
	{
		EventType Type;
		int Buttons;
		int XOffset;
		int YOffset;
	};

	union Event
	{
		EventType Type;
		KeyEvent Key;
		MouseEvent Mouse;
	};
}

#endif
