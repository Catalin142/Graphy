#pragma once

#include "Button.h"
#include "Event/Event.h"

class ButtonPanel
{
public:
	ButtonPanel(const vec3& color, const vec2& position, const vec2& size);
	~ButtonPanel() = default;

	void Render();

	bool onEvent(Event& ev);

	void setButtonSize(const vec2& size);

	void addButton(const vec3& color, const std::string& text, const std::function<void()>& func);
	void Clear();

private:
	std::vector<std::shared_ptr<Button>> m_Buttons;

	vec2 m_ButtonSize = { 10.0f, 10.0f };

	vec2 m_Position;
	vec2 m_Size;
	vec3 m_Color;

	vec2 m_ButtonPosition;

	int m_ScrollIndex = 0;
	int m_MaxButtonsOnPage = 0;
	int m_SelectedIndex = -1;
	int m_ActiveItem = -1;
};