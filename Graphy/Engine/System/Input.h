#pragma once
#include "Maths/Maths.h"

class Input
{
public:
	static bool isPressed(int code);
	static vec2 getMousePosition();
	static vec2 WindowToBufferCoordonates(const vec2& vec);

	static void blockInput(bool l);

private:
	Input() = default;
	~Input() = default;

	static bool m_InputBlocked;
};

#define VK_MOUSE_LEFT 1
#define VK_MOUSE_RIGHT 2
#define VK_MOUSE_MIDDLE 3