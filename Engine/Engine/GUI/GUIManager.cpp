#include "Core/GrPch.h"

#include "GUIManager.h"
#include "Maths/Maths.h"
#include "System/Input.h"

GUIM::~GUIM()
{
	for (auto& elem : m_GUIElements)
		delete elem.second;
	m_GUIElements.clear();
}

void GUIM::Add(const std::string& name, GUIBaseElement* elem)
{
	if (m_GUIElements.find(name) != m_GUIElements.end())
	{
		GR_WARN(name, " exista deja in GUI");
		return;
	}

	m_GUIElements[name] = elem;
}

void GUIM::Delete(const std::string& element)
{
	if (m_GUIElements.find(element) == m_GUIElements.end())
	{
		return;
	}

	delete m_GUIElements.find(element)->second;
	m_GUIElements.erase(element);
}

void GUIM::Render()
{
	for (const auto& elem : m_GUIElements)
		if (!elem.second->m_IsHidden)
			elem.second->Render();
}

bool GUIM::onEvent(Event& ev)
{
	if (ev.getType() == EventType::MousePressed)
	{
		auto& mp = static_cast<MousePressedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		for (const auto& elem : m_GUIElements)
			if (elem.second->m_isActive && elem.second->onMousePressed(mousePos.x, mousePos.y))
				return true;

		return false;
	}

	else if (ev.getType() == EventType::MouseMoved)
	{
		auto& mp = static_cast<MouseMovedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		for (const auto& elem : m_GUIElements)
			if (elem.second->m_isActive && elem.second->onMouseMoved(mousePos.x, mousePos.y))
				return true;

		return false;
	}

	else if (ev.getType() == EventType::KeyPressed)
	{
		auto& lp = static_cast<KeyPressedEvent&>(ev);
		for (const auto& elem : m_GUIElements)
			if (elem.second->m_isActive && elem.second->onKeyDown(lp.getKeyCode()))
				return true;

		return false;
	}

	return false;
}
