#pragma once
#include "System/GraphicsContext.h"
#include "System/Window.h"
#include "Types.h"
#include "Layer.h"

struct Time
{
	static float deltaTime;
};

/* 
E facuta sa faca mai usoara folosirea API-ului. In Run calculeaza intervalul de timp dintre frame-uri si face chestii pe care fiecare layer trebuie sa le faca
(sa updateze bufferul cu pixeli si sa imi actioneze la eventuri)
*/

class Application
{
	friend class Layer;
	friend class Entity;
	friend class Input;

public:
	Application(const wchar_t* title, u32 width = 1600, u32 height = 900, u32 bufferWidth = 320, u32 bufferHeight = 200, int flags = WS_OVERLAPPEDWINDOW);
	virtual ~Application();

	void setLayer(Layer* layer);

	void Run();

	std::shared_ptr<Window>& getWindow() { return m_Window; }
	std::shared_ptr<GraphicsContext>& getBuffer() { return m_Buffer; }

	static Application* Get() { return m_Instance; }

protected:
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<GraphicsContext> m_Buffer;

	Layer* m_CurrentLayer = nullptr;

	static Application* m_Instance;
};