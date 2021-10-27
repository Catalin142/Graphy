#include "Core/GrPch.h"
#include "Window.h"
#include "Event/EventDispatcher.h"
#include "Input.h"

Window* Window::m_Instance = nullptr;

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		auto window = Window::Get();
		window->m_Width = LOWORD(lParam);
		window->m_Height = HIWORD(lParam);

	} break;

	/*
	Aici creez eventul.
	Fac un obiect de tipul de care am nevoie ii dau informatile pe care le primesc de la sistem si il trimit in dispatcher.
	Cand il trimit bool-ul din clasa Event (m_Handled) e fals ca default.
	*/
	case WM_KEYDOWN:
	{
		KeyPressedEvent ev(wParam);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_LBUTTONDOWN:
	{
		vec2 pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		MousePressedEvent ev(VK_MOUSE_LEFT, pt.x, pt.y);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_RBUTTONDOWN:
	{
		vec2 pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		MousePressedEvent ev(VK_MOUSE_RIGHT, pt.x, pt.y);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_LBUTTONUP:
	{
		vec2 pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		MouseReleasedEvent ev(VK_MOUSE_LEFT, pt.x, pt.y);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_RBUTTONUP:
	{
		vec2 pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		MouseReleasedEvent ev(VK_MOUSE_RIGHT, pt.x, pt.y);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_MOUSEMOVE:
	{
		vec2 pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		MouseMovedEvent ev(pt.x, pt.y);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_MOUSEWHEEL:
	{
		int dy;
		dy = GET_WHEEL_DELTA_WPARAM(wParam);

		MouseScrollEvent ev(dy);
		EventDispatcher::Dispatch(ev);
	} break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	} break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} break;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(const wchar_t* name, u16 width, u16 height, unsigned long flags) : m_Name(const_cast<wchar_t*>(name)), m_Width(width), m_Height(height), m_isRunning(true)
{
	m_Instance = this;

	m_WindowClass = { };

	// Creez clasa si fereastra
	HINSTANCE hInstance = GetModuleHandle(NULL);

	m_WindowClass.lpfnWndProc = WindowProcedure;
	m_WindowClass.hInstance = GetModuleHandle(NULL);
	m_WindowClass.lpszClassName = L"Graphy Class";

	RegisterClass(&m_WindowClass);

	m_WindowHandle = CreateWindowEx(0, L"Graphy Class", m_Name, flags,
		CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height,
		NULL, NULL, hInstance, NULL);

	if (!m_WindowHandle)
		GR_FATAL("Nu s-a putut creea fereastra");

	m_DeviceContext = GetDC(m_WindowHandle);

	ShowWindow(m_WindowHandle, SW_SHOW);
}

void Window::pollEvents()
{
	MSG msg = { };
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	if (msg.message == WM_QUIT)
	{
		m_isRunning = false;
	}

	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

vec2 Window::getClientRectSize()
{
	RECT rct;
	GetClientRect(Window::Get()->getHandle(), &rct);
	return { (float)rct.right, (float)rct.bottom };
}
