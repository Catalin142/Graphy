#include "Core/GrPch.h"
#include "Button.h"
#include "Renderer/Renderer.h"

Button::Button(const std::shared_ptr<Texture>& tex, const vec2& position, const vec2& size) : m_Frame(tex), m_Position(position), m_Size(size),
m_PixelDimensions(m_Size.x / tex->getWidth(), m_Size.y / tex->getHeight()), m_HoverPosition(m_Position + 5.0f), m_NormalPosition(position)
{ }
void Button::Render()
{
	if (m_Position == m_HoverPosition)
		Renderer::renderTexture(m_Frame, m_NormalPosition, m_PixelDimensions, { 0.0f, 0.0f, 0.0f });

	Renderer::renderTexture(m_Frame, m_Position, m_PixelDimensions);
	Renderer::drawText(m_Text, m_TextPosition, 2, TextColor);
}

bool Button::onMousePressed(const vec2& position)
{
	if (position.x > m_NormalPosition.x && position.x < m_NormalPosition.x + m_Size.x &&
		position.y > m_NormalPosition.y && position.y < m_NormalPosition.y + m_Size.y)
	{
		m_Callback();
		return true;
	}
	return false;
}

bool Button::onMouseMoved(const vec2& position)
{
	if (position.x > m_NormalPosition.x && position.x < m_NormalPosition.x + m_Size.x &&
		position.y > m_NormalPosition.y && position.y < m_NormalPosition.y + m_Size.y)
	{
		m_Position = m_HoverPosition;
		setText(m_Text, (TextAnchorFlags)m_Flags);
		return true;
	}

	else if (!(m_Position == m_NormalPosition))
	{
		m_Position = m_NormalPosition;
		setText(m_Text, (TextAnchorFlags)m_Flags);
	}
	
	return false;
}

void Button::setText(const std::string& text, TextAnchorFlags flags)
{
	m_Text = text;
	m_Flags = flags;

	uint currentTextSize = Font::getTextWidth(m_Text, 2);

	m_TextPosition = m_Position;

	if (m_Flags & Center)
	{
		m_TextPosition.x = m_Position.x + m_Size.x / 2.0f - currentTextSize / 2.0f;
		m_TextPosition.y = m_Position.y + m_Size.y / 2.0f - Font::getGlyphHeight();
	}

	if (m_Flags & Bottom)
		m_TextPosition.y = m_Position.y + Font::getGlyphHeight();

	else if (m_Flags & Top)
		m_TextPosition.y = m_Position.y + m_Size.y - Font::getGlyphHeight() * 2.0f;

	if (m_Flags & Left)
		m_TextPosition.x = m_Position.x;

	else if (m_Flags & Right)
		m_TextPosition.x = m_Position.x + m_Size.x - currentTextSize;
}
