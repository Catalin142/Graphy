#include "Core/GrPch.h"
#include "TreeEditor.h"

#include "../Graphy/Node.h"
#include "MainMenu.h"
#include "TheoryTab.h"

void TreeEditor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseReleasedEvent>();
	Subscribe<MouseMovedEvent>();
	Subscribe<KeyPressedEvent>();

	std::shared_ptr<Texture> m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

	GUIManager.Add("Ori", new Button(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 170.0f), vec2(200.0f, 30.0f)));
	GUIManager.Get<Button>("Ori")->setText("Graf orientat", Center);
	GUIManager.Get<Button>("Ori")->setCallback([&]() -> void {
		m_GraphType = GraphType::Oriented;
		m_Graph = std::make_shared<Tree>(m_GraphType);

		InitializeGUI();
		});

	GUIManager.Add("Neo", new Button(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 120.0f), vec2(200.0f, 30.0f)));
	GUIManager.Get<Button>("Neo")->setText("Graf neorientat", Center);
	GUIManager.Get<Button>("Neo")->setCallback([&]() -> void {
		m_GraphType = GraphType::Unoriented;
		m_Graph = std::make_shared<Tree>(m_GraphType);

		InitializeGUI();
		});
}

void TreeEditor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	GUIManager.Render();

	if (m_GraphType != GraphType::None)
	{
		if (Input::isPressed('W') && m_Graph->m_SelectedNode)
		{
			m_Graph->m_SelectedNode->m_HoveredColor = vec3(1.0f, 0.0f, 0.0f);
			m_Graph->m_Select = true;
		}

		else if (m_Graph->m_SelectedNode)
		{
			if (m_Graph->m_Select)
				m_Graph->m_SelectedNode->m_isHovered = false;

			m_Graph->m_SelectedNode->m_HoveredColor = m_Graph->m_SelectedNode->m_Color;
			m_Graph->m_Select = false;
		}

		if (m_Graph->m_Select)
		{
			m_Graph->m_SelectedNode->m_isHovered = true;

			auto mousePos = Input::WindowToBufferCoordonates(Input::getMousePosition());
			float rot = std::atan2(mousePos.y - m_Graph->m_SelectedNode->m_Position.y, mousePos.x - m_Graph->m_SelectedNode->m_Position.x);

			vec2 dir = vec2(cos(rot), sin(rot));

			vec2 linePosBeg = m_Graph->m_SelectedNode->m_Position + dir * m_Graph->m_SelectedNode->m_Radius;
			vec2 linePosEnd = mousePos;

			Renderer::drawLine(linePosBeg, linePosEnd, 2.0f, vec3(0.0f, 0.0f, 0.0f));

			if (m_Graph->m_Type == GraphType::Oriented)
			{
				rot -= degToRad(30.0f);
				Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), 2.0f, vec3(0.0f, 0.0f, 0.0f));

				rot += degToRad(60.0f);
				Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), 2.0f, vec3(0.0f, 0.0f, 0.0f));
			}
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			m_LastChange = 0.0f;
			m_CurrentTip++;
			if (m_CurrentTip > TheoryTab::m_Theory[m_GraphType].size() - 1)
				m_CurrentTip = 0;
			setTip();
		}

		else if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			m_LastChange = 0.0f;
			m_CurrentTip--;
			if (m_CurrentTip < 0)
				m_CurrentTip = TheoryTab::m_Theory[m_GraphType].size() - 1;
			setTip();
		}

		if (m_LastChange > m_ChangeInterval)
		{
			m_LastChange = 0.0f; 
			m_CurrentTip++;
			if (m_CurrentTip > TheoryTab::m_Theory[m_GraphType].size() - 1)
				m_CurrentTip = 0;
			setTip();
		}
		m_LastChange += deltaTime;

		bool state = Input::isPressed(VK_SPACE);

		if (state != m_SpaceState && state == true)
		{
			vec2 mousePos = Input::WindowToBufferCoordonates(Input::getMousePosition());
			m_Graph->addNode(mousePos.x, mousePos.y);
			m_SpaceState = true;
		}
		else if (state == false) m_SpaceState = false;

		Renderer::drawLine({ 0.0f, m_BufferDim.y - m_LineOffset },
			{ (float)m_BufferDim.x, m_BufferDim.y - m_LineOffset }, 0x0000000);

		Renderer::drawLine({ m_Graph->m_MatrixPosition.x + 7 * 7.0f + 2, m_BufferDim.y - m_LineOffset }, { m_Graph->m_MatrixPosition.x + 7 * 7.0f + 2, m_BufferDim.y }, 0x000000);
		Renderer::drawLine({ m_Graph->m_MatrixPosition.x + 395.0f, m_BufferDim.y - m_LineOffset }, { m_Graph->m_MatrixPosition.x + 395.0f, m_BufferDim.y }, 0x000000);

		if (m_Graph->m_SelectedNode)
			drawNodeProps();
		else
		{
			GUIManager.Get<InputBox>("Input")->Enable(false);
		}

		m_Graph->Render();
		drawMatrix();

		if (m_Graph->m_SelectedNode && GetAsyncKeyState('R') & 0x0001)
		{
			GUIManager.Get<InputBox>("Input")->Activate();
		}
	}
}

bool TreeEditor::onEvent(Event& ev)
{
	if (m_GraphType == GraphType::None)
	{
		if (GUIManager.onEvent(ev))
		{
			return true;
		}
	}

	else
	{
		if (ev.getType() == EventType::MouseReleased)
		{
			auto mp = static_cast<MouseReleasedEvent&>(ev);
			if (m_Graph->m_SelectedNode)
			{
				if (m_Graph->m_SelectedNode->m_isMoved == true)
					m_Graph->m_SelectedNode->m_isMoved = false;
			}
		}

		else if (GUIManager.onEvent(ev))
			return true;

		else
		{
			auto type = m_Graph->onEvent(ev);
			if (type != NodeEvent::None)
			{
				if (type == NodeEvent::Select)
				{
					GUIManager.Get<InputBox>("Input")->setText(m_Graph->m_SelectedNode->m_Name, Center);
					m_Graph->m_SelectedNode->Refresh();

				}
					return true;
			}
		}
	}

	return false;
}

void TreeEditor::InitializeGUI()
{
	GUIManager.Delete("Neo");
	GUIManager.Delete("Ori");

	auto fr = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");
	GUIManager.Add("Return", new Button(fr, vec2(5.0f, m_BufferDim.y - m_LineOffset - 5.0f - fr->getHeight()),
		vec2(fr->getWidth(), fr->getHeight())));
	GUIManager.Get<Button>("Return")->setTextSize(1);
	GUIManager.Get<Button>("Return")->setHoverAnimationDist(2.5f);
	GUIManager.Get<Button>("Return")->setText("Inapoi", Center);
	GUIManager.Get<Button>("Return")->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});

	GUIManager.Add("Input", new InputBox(vec3(0.6f, 0.6f, 0.6f), 18, 7));
	GUIManager.Get<InputBox>("Input")->setPosition(m_Graph->m_MatrixPosition + vec2(400.0f, 0.0f) + vec2(Font::getTextWidth("Nod: ") + 1.0f, 0.0f));
	GUIManager.Get<InputBox>("Input")->setCharacterLimit(3);
	GUIManager.Get<InputBox>("Input")->setCharacterType(Digit);

	GUIManager.Add("Tips", new TextBox(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f),
		(m_Graph->m_MatrixPosition.x + 395.0f) - (m_Graph->m_MatrixPosition.x + 7 * 7.0f + 2) - 2,
		m_LineOffset - 3));
	GUIManager.Get<TextBox>("Tips")->setPosition({m_Graph->m_MatrixPosition.x + 7 * 7.0f + 4, m_BufferDim.y - m_LineOffset + 2});
	GUIManager.Get<TextBox>("Tips")->setOffset(7, 0);
	setTip();
}

void TreeEditor::setTip()
{
	GUIManager.Get<TextBox>("Tips")->setText(TheoryTab::m_Theory[m_GraphType][m_CurrentTip], Top | Left, { 0.0f, 0.0f, 0.0f });
	GUIManager.Get<TextBox>("Tips")->setText(std::to_string(m_CurrentTip + 1) + "\\" + std::to_string(TheoryTab::m_Theory[m_GraphType].size()), Bottom | Right, {0.0f, 0.0f, 0.0f}, false);
}

void TreeEditor::drawMatrix()
{
	int Limit;
	if (Input::isPressed(VK_TAB))
	{
		Limit = m_Graph->m_Nodes.size();
		Renderer::drawQuad({ m_Graph->m_MatrixPosition.x - 1.0f,  m_Graph->m_MatrixPosition.y - (Limit - 1) * 7.0f }, { Limit * 7.0f, Limit * 7.0f }, { 1.0f, 1.0f, 1.0f });
	}

	else Limit = Clamp(m_Graph->m_Nodes.size(), 0, 7);

	auto pos = m_Graph->m_MatrixPosition;
	for (int x = 0; x < Limit; x++)
	{
		pos.x = m_Graph->m_MatrixPosition.x;
		for (int y = 0; y < Limit; y++)
		{
			Renderer::drawNumber((char)m_Graph->m_Matrix[x][y] + '0', pos, 1, 0x000000);
			pos.x += 7.0f;
		}
		pos.y -= Font::getGlyphHeight();
	}
}

void TreeEditor::drawNodeProps()
{
	auto pos = m_Graph->m_MatrixPosition;
	pos.x += 400.0f;

	GUIManager.Get<InputBox>("Input")->Enable(true);
	m_Graph->m_SelectedNode->Refresh();

	Renderer::drawText("Nod: ", pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("ID [in matrice]: " + std::to_string(m_Graph->m_SelectedNode->m_ID), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad interior: " + std::to_string(m_Graph->m_SelectedNode->m_InternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad exterior: " + std::to_string(m_Graph->m_SelectedNode->m_ExternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();
}
