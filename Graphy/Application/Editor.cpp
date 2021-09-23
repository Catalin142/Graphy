#include "Core/GrPch.h"
#include "Editor.h"

void Editor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseReleasedEvent>();
	Subscribe<MouseMovedEvent>();
	Subscribe<KeyPressedEvent>();

	Renderer::setClearColor(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f);
	m_Graph = std::make_shared<Graph>(GraphType::Oriented);
}

void Editor::onUpdate(float deltaTime)
{
	Renderer::Clear();
	m_Graph->Update();

	bool state = Input::isPressed(VK_SPACE);

	if (state != m_SpaceState && state == true)
	{
		vec2 mousePos = Input::WindowToBufferCoordonates(Input::getMousePosition());
		m_Graph->addNode(mousePos.x, mousePos.y);
		m_SpaceState = true;
	}
	else if (state == false) m_SpaceState = false;
}

bool Editor::onEvent(Event& ev)
{
	return m_Graph->onEvent(ev);
}