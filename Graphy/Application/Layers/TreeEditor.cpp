#include "Core/GrPch.h"
#include "TreeEditor.h"

#include "../../Graphy/Node.h"
#include "MainMenu.h"
#include "TheoryTab.h"

#include "../Tools/TreeSerializer.h"
#include "../Manager.h"

void TreeEditor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseReleasedEvent>();
	Subscribe<MouseMovedEvent>();
	Subscribe<KeyPressedEvent>();

	m_MatrixPosition.x = 5.0f;
	m_MatrixPosition.y = m_BufferDim.y - Font::getGlyphHeight() - 3.0f;

	if (!m_Graph)
	{
		std::shared_ptr<Texture> m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

		GUIManager.Add("Name", new InputBox(m_TreeName, { 0.6f, 0.6f, 0.6f }, 120.0f, 20.0f));
		GUIManager.Get<InputBox>("Name")->setCharacterLimit(16);
		GUIManager.Get<InputBox>("Name")->setCharacterType(Char | Digit);
		GUIManager.Get<InputBox>("Name")->setPosition({ m_BufferDim.x / 2.0f - 60.0f, 230.0f });
		GUIManager.Get<InputBox>("Name")->setDefaultText("Introdu numele grafului");

		GUIManager.Add("Ori", new Button(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 170.0f), vec2(200.0f, 30.0f)));
		GUIManager.Get<Button>("Ori")->setText("Graf orientat", Center);
		GUIManager.Get<Button>("Ori")->setCallback([&]() -> void {
			if (!m_TreeName.empty())
			{
				if (TreeManager::Get().find(m_TreeName) != TreeManager::Get().end())
					m_TreeExists = true;
				else m_Graph = std::make_shared<Tree>(GraphType::Oriented);
			}
			});

		GUIManager.Add("Neo", new Button(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 120.0f), vec2(200.0f, 30.0f)));
		GUIManager.Get<Button>("Neo")->setText("Graf neorientat", Center);
		GUIManager.Get<Button>("Neo")->setCallback([&]() -> void {
			if (!m_TreeName.empty())
			{
				if (TreeManager::Get().find(m_TreeName) != TreeManager::Get().end())
					m_TreeExists = true;
				else m_Graph = std::make_shared<Tree>(GraphType::Oriented);
			}
			});

		GUIManager.Add("Back", new Button(vec3(0.7f, 0.7f, 0.7f), vec2(5.0f, m_BufferDim.y - 13.0f), vec2(45.0f, 8.0f)));
		GUIManager.Get<Button>("Back")->setTextSize(1);
		GUIManager.Get<Button>("Back")->setText("Inapoi", Center);
		GUIManager.Get<Button>("Back")->setHoverAnimationDist(1.0f);
		GUIManager.Get<Button>("Back")->setBorderColor({ 0.0f, 0.0f, 0.0f });
		GUIManager.Get<Button>("Back")->setCallback([&]() -> void {
			Application::Get()->setLayer(new MainMenu());
			});

	}
}

void TreeEditor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	GUIManager.Render();

	if (m_Graph)
	{
		if (!m_GUIInitialized)
		{
			InitializeGUI();
			m_GUIInitialized = true;
		}

		if (Input::isPressed(VK_CONTROL))
			if (Input::isPressed('S'))
				TreeManager::saveTree(m_TreeName, m_Graph);

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

			Renderer::drawLine(linePosBeg, linePosEnd, vec3(0.0f, 0.0f, 0.0f), 2.0f);

			if (m_Graph->m_Type == GraphType::Oriented)
			{
				rot -= degToRad(30.0f);
				Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), vec3(0.0f, 0.0f, 0.0f), 2.0f);

				rot += degToRad(60.0f);
				Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), vec3(0.0f, 0.0f, 0.0f), 2.0f);
			}
		}

		if (!m_Graph->m_Modes[Modes::Dijsktra])
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
			{
				m_LastChange = 0.0f;
				m_CurrentTip++;
				if (m_CurrentTip > TheoryTab::m_Theory[m_Graph->m_Type].size() - 1)
					m_CurrentTip = 0;
				setTip();
			}

			else if (GetAsyncKeyState(VK_LEFT) & 0x0001)
			{
				m_LastChange = 0.0f;
				m_CurrentTip--;
				if (m_CurrentTip < 0)
					m_CurrentTip = TheoryTab::m_Theory[m_Graph->m_Type].size() - 1;
				setTip();
			}

			if (m_LastChange > m_ChangeInterval)
			{
				m_LastChange = 0.0f;
				m_CurrentTip++;
				if (m_CurrentTip > TheoryTab::m_Theory[m_Graph->m_Type].size() - 1)
					m_CurrentTip = 0;
				setTip();
			}
			m_LastChange += deltaTime;

			if (m_Graph->m_SourceNode != -1)
				m_Graph->m_SourceNode = -1;

			if (m_Graph->m_DestinationNode != -1)
				m_Graph->m_DestinationNode = -1;
		}
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

		Renderer::drawLine({ m_MatrixPosition.x + 7 * 7.0f + 2, m_BufferDim.y - m_LineOffset }, { m_MatrixPosition.x + 7 * 7.0f + 2, m_BufferDim.y }, 0x000000);
		Renderer::drawLine({ m_MatrixPosition.x + 395.0f, m_BufferDim.y - m_LineOffset }, { m_MatrixPosition.x + 395.0f, m_BufferDim.y }, 0x000000);

		Renderer::drawLine({ m_BufferDim.x - 80.0f, m_BufferDim.y }, { m_BufferDim.x - 80.0f, m_BufferDim.y - m_LineOffset }, 0x0000000);

		Renderer::drawText("Ponderi", { m_BufferDim.x - 55.0f, m_BufferDim.y - 10.0f }, 1, { 0.0f, 0.0f, 0.0f });
		Renderer::drawText("Dijsktra", { m_BufferDim.x - 55.0f, m_BufferDim.y - 20.0f }, 1, { 0.0f, 0.0f, 0.0f });

		if (m_Graph->m_SelectedNode)
			drawNodeProps();
		else
			GUIManager.Get<InputBox>("Input")->Enable(false);

		m_Graph->Render();
		drawMatrix();

		bool rstate = Input::isPressed('R');
		if (m_Graph->m_SelectedNode && (rstate != m_RState && rstate == true))
		{
			GUIManager.Get<InputBox>("Input")->Activate(!GUIManager.Get<InputBox>("Input")->isActive());
			m_RState = true;
		}
		else if (rstate == false) m_RState = false;

		if (m_Graph->m_SelectedNode)
		{
			if (GUIManager.Get<InputBox>("Input")->isActive())
			{
				m_Graph->m_SelectedNode->m_Color = { 0.5f, 0.05f, 0.0f };
				m_Graph->m_SelectedNode->m_Name = m_NodeName;
				m_Graph->m_SelectedNode->Refresh();
			}
			else
				m_Graph->m_SelectedNode->m_Color = { 0.0f, 0.0f, 0.0f };
		}

		if (TreeManager::Get().find(m_TreeName) == TreeManager::Get().end())
			Renderer::drawText("Nu-i salvat!", { 405.0f,  m_BufferDim.y - m_LineOffset + 2.0f}, 1, { 1.0f, 0.0f, 0.0f });

		if (m_Graph->m_Modes[Modes::Dijsktra])
		{
			if (Input::isPressed('Z'))
				m_Graph->m_SourceNode = -1;
			if (Input::isPressed('X'))
				m_Graph->m_DestinationNode = -1;


			if (GUIManager.Get<TextBox>("Tips")->isHidden())
			{
				if (m_Graph->m_SourceNode == -1)
					Renderer::plotQuad(m_MatrixPosition + vec2(130.0f, -15.0f), { 31.0f, 16.0f }, 0xff0000);
				else if (m_Graph->m_SourceNode != -1)
				{
					Renderer::plotQuad(m_MatrixPosition + vec2(130.0f, -15.0f), { 31.0f, 16.0f }, 0x000000);
					Renderer::drawText(m_Graph->getNode(m_Graph->m_SourceNode)->m_Name, m_MatrixPosition + vec2(145.0f, -10.0f), 1, vec3(0.0f, 0.0f, 0.0f));
					Renderer::drawText("Z", m_MatrixPosition + vec2(145.0f, -25.0f), 1, vec3(0.0f, 0.0f, 0.0f));
				}

				if (m_Graph->m_DestinationNode == -1)
					Renderer::plotQuad(m_MatrixPosition + vec2(290.0f, -15.0f), { 31.0f, 16.0f }, 0xff0000);
				else
				{
					Renderer::plotQuad(m_MatrixPosition + vec2(290.0f, -15.0f), { 31.0f, 16.0f }, 0x000000);
					Renderer::drawText(m_Graph->getNode(m_Graph->m_DestinationNode)->m_Name, m_MatrixPosition + vec2(305.0f, -10.0f), 1, vec3(0.0f, 0.0f, 0.0f));
					Renderer::drawText("X", m_MatrixPosition + vec2(305.0f, -25.0f), 1, vec3(0.0f, 0.0f, 0.0f));
				}
			}
		}
	}
	else
	{
		if (m_TreeExists)
		{
			Renderer::drawText("Graf deja existent", { m_BufferDim.x / 2.0f - Font::getTextWidth("Graf deja existent") / 2.0f, 220.0f }, 1, vec3(1.0f, 0.0f, 0.0f));
		}

	}
}

bool TreeEditor::onEvent(Event& ev)
{
	if (m_Graph)
	{
		GUIManager.Get<TextBox>("Tips")->Hide(m_Graph->m_Modes[Modes::Dijsktra]);
		GUIManager.Get<Button>("Generate")->Enable(m_Graph->m_Modes[Modes::Dijsktra]);

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

	else
	{
		if (GUIManager.onEvent(ev))
			return true;
	}

	return false;
}

void TreeEditor::InitializeGUI()
{
	Input::blockInput(false);
	GUIManager.Delete("Neo");
	GUIManager.Delete("Ori");
	GUIManager.Delete("Name");
	GUIManager.Delete("Back");

	auto fr = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");
	GUIManager.Add("Return", new Button(fr, vec2(5.0f, m_BufferDim.y - m_LineOffset - 5.0f - fr->getHeight()),
		vec2(fr->getWidth(), fr->getHeight())));
	GUIManager.Get<Button>("Return")->setTextSize(1);
	GUIManager.Get<Button>("Return")->setHoverAnimationDist(2.5f);
	GUIManager.Get<Button>("Return")->setText("Inapoi", Center);
	GUIManager.Get<Button>("Return")->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});

	GUIManager.Add("Input", new InputBox(m_NodeName, vec3(0.6f, 0.6f, 0.6f), 18, 7));
	GUIManager.Get<InputBox>("Input")->setPosition(m_MatrixPosition + vec2(400.0f, 0.0f) + vec2(Font::getTextWidth("Nod: ") + 1.0f, 0.0f));
	GUIManager.Get<InputBox>("Input")->setCharacterLimit(3);
	GUIManager.Get<InputBox>("Input")->setCharacterType(Digit);

	GUIManager.Add("Tips", new TextBox(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f),
		(m_MatrixPosition.x + 395.0f) - (m_MatrixPosition.x + 7 * 7.0f + 2) - 2,
		m_LineOffset - 3));
	GUIManager.Get<TextBox>("Tips")->setPosition({m_MatrixPosition.x + 7 * 7.0f + 4, m_BufferDim.y - m_LineOffset + 2});
	GUIManager.Get<TextBox>("Tips")->setOffset(7, 0);
	setTip();

	vec2 checkPosFirst = { m_BufferDim.x - 70.0f, m_BufferDim.y - 10.0f };
	GUIManager.Add("Length", new CheckBox(m_Graph->m_Modes[Modes::Length], checkPosFirst, { 7.0f, 7.0f }, { 0.0f, 0.0f, 0.0f }));
	checkPosFirst.y -= 10.0f;
	GUIManager.Add("Dijsktra", new CheckBox(m_Graph->m_Modes[Modes::Dijsktra], checkPosFirst, { 7.0f, 7.0f }, { 0.0f, 0.0f, 0.0f }));

	m_Graph->m_SelectedNode = nullptr;

	GUIManager.Add("Generate", new Button(vec3(1.0f, 1.0f, 1.0f), m_MatrixPosition + vec2(200.0f, -35.0f), vec2(50, 14)));
	GUIManager.Get<Button>("Generate")->setTextSize(1);
	GUIManager.Get<Button>("Generate")->setHoverAnimationDist(1.0f);
	GUIManager.Get<Button>("Generate")->setText("Genereaza", Center);
	GUIManager.Get<Button>("Generate")->setBorderColor(vec3(0.0f, 0.0f, 0.0f));
	GUIManager.Get<Button>("Generate")->setCallback([&]() -> void {
		m_Graph->refreshAlgorithms();
		});
	GUIManager.Get<Button>("Generate")->Enable(false);

}

void TreeEditor::setTip()
{
	GUIManager.Get<TextBox>("Tips")->setText(TheoryTab::m_Theory[m_Graph->m_Type][m_CurrentTip], Top | Left, { 0.0f, 0.0f, 0.0f });
	GUIManager.Get<TextBox>("Tips")->setText(std::to_string(m_CurrentTip + 1) + "\\" + std::to_string(TheoryTab::m_Theory[m_Graph->m_Type].size()), Bottom | Right, {0.0f, 0.0f, 0.0f}, false);
}

void TreeEditor::drawMatrix()
{
	int Limit;
	if (Input::isPressed(VK_TAB))
	{
		Limit = m_Graph->m_Nodes.size();
		Renderer::drawQuad({ m_MatrixPosition.x - 1.0f,  m_MatrixPosition.y - (Limit - 1) * 7.0f }, { Limit * 7.0f, Limit * 7.0f }, { 1.0f, 1.0f, 1.0f });
	}

	else Limit = Clamp(m_Graph->m_Nodes.size(), 0, 7);

	auto pos = m_MatrixPosition;
	for (int x = 0; x < Limit; x++)
	{
		pos.x = m_MatrixPosition.x;
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
	auto pos = m_MatrixPosition;
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
	pos.y -= Font::getGlyphHeight() * 2.0f;
}
