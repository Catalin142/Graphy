#include "Core/GrPch.h"

#include "Input.h"
#include "System/Window.h"
#include "Core/Application.h"

bool Input::m_InputBlocked = false;

bool Input::isPressed(int code)
{
	if (!m_InputBlocked)
		return GetAsyncKeyState(code);
	else return false;
}

vec2 Input::getMousePosition()
{
	POINT p = { 0.0, 0.0 };
	GetCursorPos(&p);
	ScreenToClient(Window::Get()->getHandle(), &p);

	return vec2((float)p.x, (float)p.y);
}

vec2 Input::WindowToBufferCoordonates(const vec2& vec)
{
	vec2 normalPos = vec;
	auto app = Application::Get();

	vec2 norm = { (float)app->m_Buffer->getWidth() / (float)app->m_Window->getWidth(), (float)app->m_Buffer->getHeight() / (float)app->m_Window->getHeight() };
	normalPos = normalPos * norm;

	normalPos.y = app->m_Buffer->getHeight() - normalPos.y;

	return normalPos;
}

void Input::blockInput(bool l)
{
	m_InputBlocked = l;
}
