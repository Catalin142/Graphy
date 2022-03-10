#include "Core/GrPch.h"

#include "CheckBox.h"
#include "Renderer/Renderer.h"
CheckBox::CheckBox(bool& flag, const vec2& pos, const vec2& size, const vec3& color) : m_Active(flag), m_Position(pos), m_Size(size), m_Color(color)
{ }

void CheckBox::Render()
{
	if (m_Active)
		Renderer::drawQuad(m_Position, m_Size, m_Color);
	else Renderer::plotQuad(m_Position, m_Size, m_Color);
}

bool CheckBox::onMousePressed(int x, int y)
{
	if (x > m_Position.x && x < m_Position.x + m_Size.x &&
		y > m_Position.y && y < m_Position.y + m_Size.y)
	{
		m_Active = !m_Active;
		return true;
	}
	return false;
}
