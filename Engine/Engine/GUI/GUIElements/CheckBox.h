#pragma once

#include "GUI/GUIBase/GUIBaseElement.h"
#include "Maths/Maths.h"

class CheckBox : public GUIBaseElement
{
public:
	CheckBox(bool& flag, const vec2& pos, const vec2& size, const vec3& color);
	~CheckBox() = default;

	void Render() override;

	bool onMousePressed(int x, int y) override;
	void setPosition(float x, float y) { m_Position = { x, y }; }

	bool isActive() const { return m_Active; }

private:
	vec2 m_Position;
	vec2 m_Size;
	vec3 m_Color;

	bool& m_Active;

};