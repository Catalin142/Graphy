#include "Core/GrPch.h"
#include "ButtonPanel.h"
#include "System/Input.h"

#include "Renderer/Renderer.h"

ButtonPanel::ButtonPanel(const vec3& color, const vec2& position, const vec2& size) : m_Color(color), m_Position(position), m_Size(size)
{ }

void ButtonPanel::Render()
{
	Renderer::drawQuad(m_Position, m_Size, m_Color);
	Renderer::plotQuad(m_Position, m_Size, 0x000000);
	m_ButtonPosition = { m_Position.x + m_Size.x / 2.0f - m_ButtonSize.x / 2.0f, m_Position.y + m_Size.y - m_ButtonSize.y - 10.0f };

	if (m_Buttons.empty())
	{
		int size = Font::getTextWidth("Nimic salvat!", 2);
		Renderer::drawText("Nimic salvat!", m_Position + vec2(m_Size.x / 2.0f - size / 2.0f, m_Size.y - 20.0f), 2, { 0.0f, 0.0f, 0.0f });
	}
	else
	{
		int max = std::min((int)m_Buttons.size(), m_ScrollIndex + m_MaxButtonsOnPage);
		for (int i = m_ScrollIndex; i < max; i++)
		{
			auto button = m_Buttons[i];

			button->Render();
			if (i == m_SelectedIndex)
				Renderer::plotQuad(m_ButtonPosition, m_ButtonSize, 0xff0000, 2.0f);
			else if (i == m_ActiveItem)
				Renderer::plotQuad(m_ButtonPosition, m_ButtonSize, { 0.5f, 0.05f, 0.0f }, 2.0f);
			else
				Renderer::plotQuad(m_ButtonPosition, m_ButtonSize, 0x000000);

			m_ButtonPosition.y -= (m_ButtonSize.y + 3.0f);
		}
	}
}

bool ButtonPanel::onEvent(Event& ev)
{
	if (ev.getType() == EventType::MouseScroll)
	{
		auto ms = static_cast<MouseScrollEvent&>(ev);
		if (m_Buttons.size() > m_MaxButtonsOnPage)
		{
			if (ms.getDelta() < 0)
				m_ScrollIndex += 1;
			else if (ms.getDelta() > 0)
				m_ScrollIndex -= 1;

			if (m_ScrollIndex < 0)
				m_ScrollIndex = 0;
			else if (m_ScrollIndex > m_Buttons.size() - m_MaxButtonsOnPage)
				m_ScrollIndex = m_Buttons.size() - m_MaxButtonsOnPage;

			m_ButtonPosition = { m_Position.x + m_Size.x / 2.0f - m_ButtonSize.x / 2.0f, m_Position.y + m_Size.y - m_ButtonSize.y - 10.0f };
			int max = std::min((int)m_Buttons.size(), m_ScrollIndex + m_MaxButtonsOnPage);
			for (int i = m_ScrollIndex; i < max; i++)
			{
				m_Buttons[i]->setPosition(m_ButtonPosition.x, m_ButtonPosition.y);
				m_ButtonPosition.y -= (m_ButtonSize.y + 3.0f);
			}
		}

		return true;
	}

	else if (ev.getType() == EventType::MouseMoved)
	{
		auto mv = static_cast<MouseMovedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mv.getX(), mv.getY()));

		for(int i = 0; i < m_Buttons.size(); i++)
		{
			auto button = m_Buttons[i];
			if (button->onMouseMoved(mousePos.x, mousePos.y))
			{
				m_SelectedIndex = i;
				return true;
			}
		}
		m_SelectedIndex = -1;
	}

	else if (ev.getType() == EventType::MousePressed)
	{
		auto mv = static_cast<MousePressedEvent&>(ev);
		if (mv.getMouseCode() == VK_MOUSE_LEFT)
		{
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mv.getX(), mv.getY()));

			for (int i = 0; i < m_Buttons.size(); i++)
			{
				auto button = m_Buttons[i];
				if (button->onMousePressed(mousePos.x, mousePos.y))
				{
					m_ActiveItem = i;
					return true;
				}
			}
		}
	}

	return false;
}

void ButtonPanel::setButtonSize(const vec2& size)
{
	m_ButtonSize = size; 
	m_MaxButtonsOnPage = (m_Size.y - 10.0f) / (m_ButtonSize.y + 3.0f);
	m_ButtonPosition = { m_Position.x + m_Size.x / 2.0f - m_ButtonSize.x / 2.0f, m_Position.y + m_Size.y - m_ButtonSize.y - 10.0f };
}

void ButtonPanel::addButton(const vec3& color, const std::string& text, const std::function<void()>& func)
{
	auto button = std::make_shared<Button>(color, m_ButtonPosition, m_ButtonSize);
	button->setText(text, Center);
	button->setHoverAnimationDist(0.0f);
	button->setCallback(func);

	m_Buttons.push_back(button);
	m_ButtonPosition.y -= (m_ButtonSize.y + 3.0f);
}

void ButtonPanel::Clear()
{
	m_Buttons.clear();
	m_ButtonPosition = { m_Position.x + m_Size.x / 2.0f - m_ButtonSize.x / 2.0f, m_Position.y + m_Size.y - m_ButtonSize.y - 10.0f };
}