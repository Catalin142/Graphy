#include "Core/GrPch.h"

#include "GraphEditor.h"
#include "Renderer/Renderer.h"
#include "System/Input.h"

void GraphEditor::onAttach()
{
	m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };
}

void GraphEditor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	float beg = m_LineOffset + m_Margin;
	for (int i = 1; i < m_BufferDim.x / m_LineDistance; i++)
		Renderer::drawLine({ i * m_LineDistance, m_Margin }, { i * m_LineDistance, m_BufferDim.y - beg }, 0x808080);

	for (int i = 1; i < int(m_BufferDim.y - beg) / m_LineDistance; i++)
		if (i == (int)(m_BufferDim.y / m_LineDistance / 2.0f) - 1)
			Renderer::drawLine({ m_Margin, i * m_LineDistance }, { m_BufferDim.x - m_Margin, i * m_LineDistance }, 0x000000);
		else
			Renderer::drawLine({ m_Margin, i * m_LineDistance }, { m_BufferDim.x - m_Margin, i * m_LineDistance }, 0x808080);

	Renderer::drawLine({ int(m_BufferDim.x / m_LineDistance / 2.0f) * m_LineDistance, m_Margin }, 
		{ int(m_BufferDim.x / m_LineDistance / 2.0f) * m_LineDistance, m_BufferDim.y - beg }, 0x000000);
	
	Renderer::drawLine({ 0, m_BufferDim.y - m_LineOffset },
		{ m_BufferDim.x, m_BufferDim.y - m_LineOffset }, 0x0000000);

	Renderer::drawLine({ m_Margin, m_BufferDim.y - m_LineOffset - m_Margin },
		{ m_BufferDim.x - m_Margin, m_BufferDim.y - m_LineOffset - m_Margin }, 0x0000000);
	
	Renderer::drawLine({ m_Margin, m_Margin },
		{ m_Margin, m_BufferDim.y - m_LineOffset - m_Margin }, 0x0000000);
	
	Renderer::drawLine({ m_Margin, m_Margin },
		{ m_BufferDim.x - m_Margin, m_Margin }, 0x0000000);

	Renderer::drawLine({ m_BufferDim.x - m_Margin, m_Margin },
		{ m_BufferDim.x - m_Margin, m_BufferDim.y - m_LineOffset - m_Margin }, 0x0000000);

	vec2 mousePos = Input::WindowToBufferCoordonates(Input::getMousePosition());
	Renderer::drawCircle({ int(std::floor(mousePos.x / m_LineDistance)) * m_LineDistance, int(std::floor(mousePos.y / m_LineDistance)) * m_LineDistance}, 5.0f, 0xff0000);

	for (int i = 1; i < (int)m_BufferDim.x / m_LineDistance; i++)
	{
		int nr = std::ceil(i - (m_BufferDim.x / m_LineDistance / 2));
		int posX = i * m_LineDistance;

		if (nr < 0 || nr > 9)
			posX -= 4.0f;

		Renderer::drawText(std::to_string(nr),
			{ (float)posX, m_BufferDim.y - m_LineOffset - 7.0f }, 1, { 0.0f, 0.0f, 0.0f }); 
		
		Renderer::drawText(std::to_string(nr),
				{ (float)posX, 0 }, 1, { 0.0f, 0.0f, 0.0f });
	}

	for (int i = 1; i < int(m_BufferDim.y - beg) / m_LineDistance; i++)
	{
		int nr = std::ceil(i - (m_BufferDim.y / m_LineDistance / 2)) + 1;
		int posY = i * m_LineDistance;

		if (nr < 0 || nr > 9)
			posY -= 4.0f;

		std::string text = std::to_string(nr);
		if (nr >= 0 && nr < 10)
			text.insert(text.begin(), ' ');

		Renderer::drawText(text,
			{ (float)0, (float)posY }, 1, { 0.0f, 0.0f, 0.0f });

		Renderer::drawText(text,
			{ (float)m_BufferDim.x - m_Margin, (float)posY }, 1, { 0.0f, 0.0f, 0.0f });
	}

}

bool GraphEditor::onEvent(Event& ev)
{
	return false;
}
