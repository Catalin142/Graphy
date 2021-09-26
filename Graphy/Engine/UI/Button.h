#pragma once

#include "Maths/Maths.h"
#include "TextBox.h"

class Button
{
public:
	Button() = default;
	Button(const std::shared_ptr<Texture>& tex, const vec2& position, const vec2& size);

	~Button() = default;

	void Render();

	bool onMousePressed(const vec2& position);
	bool onMouseMoved(const vec2& position);

	void setCallback(const std::function<void()>& func) { m_Callback = func; }

	void setTextSize(int size) { m_TextSize = size; }
	void setText(const std::string& text, TextAnchorFlags flags);
	void setHoverAnimationDist(float dist) { m_HoverDist = dist, m_HoverPosition = m_Position + m_HoverDist; }

	vec3 TextColor;

private:
	std::shared_ptr<Texture> m_Frame;

	int m_Flags;
	std::string m_Text;
	int m_TextSize = 2;
	vec2 m_TextPosition;

	vec2 m_Position;
	vec2 m_HoverPosition;
	vec2 m_NormalPosition;
	float m_HoverDist = 5.0f;

	vec2 m_Size;
	vec2 m_PixelDimensions;

	std::function<void()> m_Callback;
};