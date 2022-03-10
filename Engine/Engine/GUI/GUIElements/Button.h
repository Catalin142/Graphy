#pragma once

#include "GUI/GUIBase/GUIBaseElement.h"

#include "Maths/Maths.h"
#include "TextBox.h"

class Button : public GUIBaseElement
{
	friend class ButtonPanel;

public:
	Button() = default;
	Button(const std::shared_ptr<Texture>& tex, const vec2& position, const vec2& size);
	Button(const vec3& color, const vec2& position, const vec2& size);

	~Button() = default;

	void Render() override;

	bool onMousePressed(int x, int y) override;
	bool onMouseMoved(int x, int y) override;

	bool isHovered(int x, int y);

	void setCallback(const std::function<void()>& func) { m_Callback = func; }

	void setTextSize(int size) override { m_TextSize = size; }
	void setText(const std::string& text, TextAnchorFlags flags) override;

	void setSize(int width, int height);
	void setPosition(float x, float y);
	void setBorderColor(const vec3& color) { m_BorderColor = color; }

	void setHoverAnimationDist(float dist) { m_HoverDist = dist, m_HoverPosition = m_Position + m_HoverDist; }

	vec3 TextColor = vec3(0.0f, 0.0f, 0.0f);

private:
	std::shared_ptr<Texture> m_Frame;
	vec3 m_Color;
	vec3 m_BorderColor = { -1.0f, -1.0f, -1.0f };

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

private:
	void Refresh();
};