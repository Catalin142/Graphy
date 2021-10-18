#include "Core/GrPch.h"

#include "GraphEditor.h"
#include "Renderer/Renderer.h"
#include "System/Input.h"
#include "MainMenu.h"

void GraphEditor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseMovedEvent>();

	auto& bf = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

	GUIManager.Add("Return", new Button(bf,
		vec2(5.0f, m_BufferDim.y - m_LineOffset + 4),
		vec2(bf->getWidth(), bf->getHeight())));
	GUIManager.Get<Button>("Return")->setTextSize(1);
	GUIManager.Get<Button>("Return")->setHoverAnimationDist(2.5f);
	GUIManager.Get<Button>("Return")->setText("Inapoi", Center);
	GUIManager.Get<Button>("Return")->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});
}

void GraphEditor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	GUIManager.Render();

	float beg = m_LineOffset + m_Margin;
	for (int i = 1; i < m_BufferDim.x / m_LineDistance; i++)
		Renderer::drawLine({ i * m_LineDistance, m_Margin }, { i * m_LineDistance, m_BufferDim.y - beg }, 0x808080);

	for (int i = 1; i < int(m_BufferDim.y - beg) / m_LineDistance; i++)
		if (i == (int)(m_BufferDim.y / m_LineDistance / 2.0f))
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

	bool state = Input::isPressed(VK_SPACE);

	if (state != m_SpaceState && state == true)
	{
		vec2 mousePos = Input::WindowToBufferCoordonates(Input::getMousePosition());

		mousePos.x = Clamp(mousePos.x, m_Margin + 6.0f, m_BufferDim.x - m_Margin - 6.0f);
		mousePos.y = Clamp(mousePos.y, m_Margin + 6.0f, m_BufferDim.y - m_LineOffset - m_Margin - 6.0f);

		auto node = std::make_shared<Node>(0, m_Letter, mousePos, 6.0f);
		node->m_TextSize = 1;
		node->Refresh();
		m_Points.push_back(node);

		m_Letter[0] += 1;
		if (m_Letter[0] > 'z')
			m_Letter[0] = 'a';
			
		m_SpaceState = true;
	}
	else if (state == false) m_SpaceState = false;

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
		int nr = std::ceil(i - (m_BufferDim.y / m_LineDistance / 2));
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

	for (auto& point : m_Points)
		point->Render();
}

bool GraphEditor::onEvent(Event& ev)
{
	if (GUIManager.onEvent(ev))
		return true;

	else if(ev.getType() == EventType::MousePressed)
	{
		auto mp = static_cast<MousePressedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));
		
		if (mp.getMouseCode() == VK_MOUSE_RIGHT)
		{
			if (m_CurrentPoint && m_CurrentPoint->isHovered(mousePos.x, mousePos.y))
			{
				m_Points.erase(std::find(m_Points.begin(), m_Points.end(), m_CurrentPoint));
				m_CurrentPoint = nullptr;
			}
		}
	}

	else if (ev.getType() == EventType::MouseMoved)
	{
		auto mp = static_cast<MouseMovedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		bool found = false;
		for (auto& point = m_Points.rbegin(); point != m_Points.rend(); point++)
			if ((*point)->isHovered(mousePos.x, mousePos.y))
			{
				if (found == false)
					m_CurrentPoint = *point;

				if (found == false)
					found = true;
				else (*point)->m_isHovered = false;
			}
	}

	return false;
}
