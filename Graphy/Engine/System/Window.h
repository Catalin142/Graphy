#pragma once

#pragma once
#include "Core/Types.h"
#include "Maths/Maths.h"

class Window
{
	friend LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Window(const wchar_t* name, u16 width, u16 height, unsigned long flags = WS_OVERLAPPEDWINDOW);

	void pollEvents();

	u16 getWidth() { return m_Width; }
	u16 getHeight() { return m_Height; }
	HWND& getHandle() { return m_WindowHandle; }
	HDC& getDeviceContext() { return m_DeviceContext; }

	void setWidth(u16 width) { m_Width = width; }
	void setHeight(u16 height) { m_Height = height; }
	vec2 getClientRectSize();

	bool& isRunning() { return m_isRunning; }
	std::wstring getName() { return std::wstring(m_Name); }

	static Window* Get() { return m_Instance; }

private:
	HWND m_WindowHandle;
	WNDCLASS m_WindowClass;
	HDC m_DeviceContext;

	u16 m_Width;
	u16 m_Height;
	vec2 m_ClientSize = { 0.0f, 0.0f };

	wchar_t* m_Name;
	bool m_isRunning;

	static Window* m_Instance;
};