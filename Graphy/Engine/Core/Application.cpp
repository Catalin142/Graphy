#include "Core/GrPch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Graphics/Font.h"

Application* Application::m_Instance;
float Time::deltaTime = 0.0f;

Application::Application(const wchar_t* title, u32 width, u32 height, u32 bufferWidth, u32 bufferHeight, int flags) :
	m_Window(std::make_shared<Window>(title, width, height, flags)),
	m_Buffer(std::make_shared<GraphicsContext>(m_Window, bufferWidth, bufferHeight))
{
	Font::Init("Resources/Engine/Font.spr", "Resources/Engine/Numbers.spr", "Resources/Engine/Symbols.spr", 6, 7);
	m_Instance = this;
}

void Application::setLayer(Layer* layer)
{
	if (m_CurrentLayer)
	{
		m_CurrentLayer->onDetach();
		delete m_CurrentLayer;
	}
	m_CurrentLayer = layer;
	m_CurrentLayer->onAttach();
}

void Application::Run()
{
	std::chrono::time_point<std::chrono::system_clock> timeNow = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> oldTime = timeNow;

	float lastFrameChange = 0.4f;

	Renderer::Init(m_Buffer);

	while (m_Window->isRunning())
	{
		timeNow = std::chrono::system_clock::now();
		Time::deltaTime = std::chrono::duration<float>(timeNow - oldTime).count();

		if (lastFrameChange >= 0.5f)
		{
			std::wstring title = m_Window->getName() + L": FPS " + std::to_wstring(int(1 / Time::deltaTime));
			SetWindowText(m_Window->getHandle(), title.c_str());
			lastFrameChange = 0.0f;
		} // TODO scoatel

		if (m_CurrentLayer->m_Active)
			m_CurrentLayer->onUpdate(Time::deltaTime);

		Renderer::Draw();

		lastFrameChange += Time::deltaTime;

		m_Window->pollEvents();

		if (GetAsyncKeyState(VK_ESCAPE))
			m_Window->isRunning() = false; // TODO scoatel

		oldTime = timeNow;
	}
}
