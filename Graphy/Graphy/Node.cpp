#include "Core/GrPch.h"
#include "Node.h"

#include "Graphics/Font.h"
#include "Renderer/Renderer.h"
#include "Core/Application.h"

Node::Node(int id, const std::string& number, const vec2& position, int radius) : m_ID(id), m_Name(number), m_Position(position), m_Radius(radius)
{
	Refresh();
}

void Node::Render()
{
	Renderer::drawCircle(m_Position, m_Radius, m_Color);
	if (m_isHovered)
		Renderer::plotCircle(m_Position, m_Radius + 3, m_HoveredColor);

	Renderer::drawText(m_Name, m_TextPosition, m_TextSize, { 1.0f, 1.0f, 1.0f });
}

bool Node::isPressed(int x, int y)
{
	vec2 distance = m_Position - vec2(x, y);
	if (distance.magnitude() < m_Radius)
	{
		return true;
	}

	return false;
}

bool Node::isHovered(int x, int y)
{
	vec2 distance = m_Position - vec2(x, y);
	if (distance.magnitude() < m_Radius)
	{
		m_isHovered = true;
		return true;
	}

	m_isHovered = false; 
	return false;
}

void Node::Refresh()
{
	int textSize = Font::getTextWidth(m_Name, m_TextSize);
	m_TextPosition.x = m_Position.x - textSize / 2.0f;
	m_TextPosition.y = m_Position.y - (Font::getGlyphHeight() * m_TextSize) / 2.0f;
}