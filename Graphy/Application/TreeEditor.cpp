#include "Core/GrPch.h"
#include "TreeEditor.h"
#include "../Graphy/Node.h"
#include "MainMenu.h"

void TreeEditor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseReleasedEvent>();
	Subscribe<MouseMovedEvent>();
	Subscribe<KeyPressedEvent>();

	loadTips("Resources/Editor/Tips.txt");

	m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };

	std::shared_ptr<Texture> m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

	m_OrientatButton = std::make_shared<Button>(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 170.0f), vec2(200.0f, 30.0f));
	m_OrientatButton->setText("Graf orientat", Center);
	m_OrientatButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
	m_OrientatButton->setCallback([&]() -> void {
		m_GraphType = TreeType::Oriented;
		m_Graph = std::make_shared<Tree>(m_GraphType);

		createTextBox();
		});

	m_NeoButton = std::make_shared<Button>(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 120.0f), vec2(200.0f, 30.0f));
	m_NeoButton->setText("Graf neorientat", Center);
	m_NeoButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
	m_NeoButton->setCallback([&]() -> void {
		m_GraphType = TreeType::Unoriented;
		m_Graph = std::make_shared<Tree>(m_GraphType);

		createTextBox();
		});

	m_BackButton = std::make_shared<Button>(m_ButtonFrame, vec2(5.0f, m_BufferDim.y - m_LineOffset - 5.0f -  m_ButtonFrame->getHeight()),
		vec2(m_ButtonFrame->getWidth(), m_ButtonFrame->getHeight()));
	m_BackButton->setTextSize(1);
	m_BackButton->setHoverAnimationDist(2.5f);
	m_BackButton->setText("Inapoi", Center);
	m_BackButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
	m_BackButton->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});
}

void TreeEditor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	if (m_GraphType == TreeType::None)
	{
		m_NeoButton->Render();
		m_OrientatButton->Render();
	}

	else
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

			// Dupa ce selectez un nod sa dau sageata la el si nu misc mouse-ul nu se updateaza nodul selectat asa ca trimit un event sa se updateze
			auto pt = Input::getMousePosition();
			MouseMovedEvent ev(pt.x, pt.y);
			EventDispatcher::Dispatch(ev);
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

			if (m_Graph->m_Type == TreeType::Oriented)
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
			if (m_CurrentTip > m_Tips[m_GraphType].size() - 1)
				m_CurrentTip = 0;
			setTip();
		}

		else if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			m_LastChange = 0.0f;
			m_CurrentTip--;
			if (m_CurrentTip < 0)
				m_CurrentTip = m_Tips[m_GraphType].size() - 1;
			setTip();
		}

		if (m_LastChange > m_ChangeInterval)
		{
			m_LastChange = 0.0f; 
			m_CurrentTip++;
			if (m_CurrentTip > m_Tips[m_GraphType].size() - 1)
				m_CurrentTip = 0;
			setTip();
		}
		m_LastChange += deltaTime;

		m_TipsBox->Render();

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

		m_BackButton->Render();

		m_Graph->Render();
		drawMatrix();

		if (m_Graph->m_SelectedNode && GetAsyncKeyState('R') & 0x0001)
		{
			m_Graph->m_SelectedNode->m_Name = "";
			m_Graph->m_InputBox->Activate();
			m_Graph->m_InputBox->setText("");
		}
	}
}

bool TreeEditor::onEvent(Event& ev)
{
	if (m_GraphType == TreeType::None)
	{
		if (ev.getType() == EventType::MousePressed)
		{
			auto mp = static_cast<MousePressedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (mp.getMouseCode() == VK_MOUSE_LEFT)
			{
				if (m_NeoButton->onMousePressed(mousePos))
					return true;

				else if (m_OrientatButton->onMousePressed(mousePos))
					return true;
			}

			return false;
		}

		else if (ev.getType() == EventType::MouseMoved)
		{
			auto mp = static_cast<MouseMovedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (m_NeoButton->onMouseMoved(mousePos))
				return true;

			else  if (m_OrientatButton->onMouseMoved(mousePos))
				return true;

			return false;
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

		else if (ev.getType() == EventType::MousePressed)
		{
			auto mp = static_cast<MousePressedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (mp.getMouseCode() == VK_MOUSE_LEFT)
			{
				if (m_BackButton->onMousePressed(mousePos))
					return true;
			}
		}

		else if (ev.getType() == EventType::MouseMoved)
		{
			auto mp = static_cast<MouseMovedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (m_BackButton->onMouseMoved(mousePos))
				return true;
		}

		m_Graph->onEvent(ev);
	}

	return false;
}

void TreeEditor::createTextBox()
{
	m_TipsBox = std::make_shared<TextBox>(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f),
		(m_Graph->m_MatrixPosition.x + 395.0f) - (m_Graph->m_MatrixPosition.x + 7 * 7.0f + 2) - 2,
		m_LineOffset - 3);
	m_TipsBox->setPosition({ m_Graph->m_MatrixPosition.x + 7 * 7.0f + 4, m_BufferDim.y - m_LineOffset + 2 });
	m_TipsBox->setOffset(7, 0);
	setTip();
}

void TreeEditor::setTip()
{
	m_TipsBox->setText(m_Tips[m_GraphType][m_CurrentTip], Top | Left, { 0.0f, 0.0f, 0.0f });
	m_TipsBox->setText(std::to_string(m_CurrentTip + 1) + "\\" + std::to_string(m_Tips[m_GraphType].size()), Bottom | Right, {0.0f, 0.0f, 0.0f}, false);
}

void TreeEditor::loadTips(const std::string& filepath)
{
	std::fstream doc(filepath);
	if (!doc.good())
		return;
	
	std::string line;
	std::string def;

	TreeType currentType;

	while (std::getline(doc, line))
	{
		if (line.find("Orientat") != std::string::npos)
		{
			if (!def.empty())
				m_Tips[currentType].push_back(def);
			def.clear();
			currentType = TreeType::Oriented;
		}
		else if (line.find("Neorientat") != std::string::npos) {
			if (!def.empty())
				m_Tips[currentType].push_back(def);
			def.clear();
			currentType = TreeType::Unoriented;
		}
		else
		{
			if (line.find("Def:") != std::string::npos)
			{
				if (!def.empty())
					m_Tips[currentType].push_back(def);
				def.clear();
			}
			else
			{
				def += line;
				def += '\n';
			}
		}
	}
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

	m_Graph->m_InputBox->setPosition(pos + vec2(Font::getTextWidth("Nod: ") + 1.0f, 0.0f));
	m_Graph->m_InputBox->setText(m_Graph->m_SelectedNode->m_Name);
	m_Graph->m_InputBox->Render();

	Renderer::drawText("Nod: ", pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("ID [in matrice]: " + std::to_string(m_Graph->m_SelectedNode->m_ID), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad interior: " + std::to_string(m_Graph->m_SelectedNode->m_InternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad exterior: " + std::to_string(m_Graph->m_SelectedNode->m_ExternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();
}
