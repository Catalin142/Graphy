#include "Core/GrPch.h"
#include "Editor.h"
#include "../Graphy/Node.h"
#include "MainMenu.h"

void Editor::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseReleasedEvent>();
	Subscribe<MouseMovedEvent>();
	Subscribe<KeyPressedEvent>();

	loadTips("Resources/Editor/Tips.txt");

	m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };

	Renderer::setClearColor(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f);

	std::shared_ptr<Texture> m_ButtonFrame = std::make_shared<Texture>("Resources/Menu/ButtonFrame.spr");

	m_OrientatButton = std::make_shared<Button>(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 170.0f), vec2(200.0f, 30.0f));
	m_OrientatButton->setText("Graf orientat", Center);
	m_OrientatButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
	m_OrientatButton->setCallback([&]() -> void {
		m_GraphType = GraphType::Oriented;
		m_Graph = std::make_shared<Graph>(m_GraphType);

		createTextBox();
		});

	m_NeoButton = std::make_shared<Button>(m_ButtonFrame, vec2(m_BufferDim.x / 2.0f - 100.0f, 120.0f), vec2(200.0f, 30.0f));
	m_NeoButton->setText("Graf neorientat", Center);
	m_NeoButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
	m_NeoButton->setCallback([&]() -> void {
		m_GraphType = GraphType::Unoriented;
		m_Graph = std::make_shared<Graph>(m_GraphType);

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

	m_ThrashBin = std::make_shared<Texture>("Resources/Editor/RecycleBin.spr");
	m_ThrashBinSize = { m_ThrashBin->getWidth() * 2.2f, m_ThrashBin->getHeight() * 2.3f };

	m_ThrashBinPos = { m_BufferDim.x - m_ThrashBinSize.x, m_BufferDim.y - m_LineOffset - m_ThrashBinSize.y };
}

void Editor::onUpdate(float deltaTime)
{
	Renderer::Clear();

	if (m_GraphType == GraphType::None)
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

		Renderer::renderTexture(m_ThrashBin, m_ThrashBinPos, { 2.0f, 2.0f });
		Renderer::plotQuad(m_ThrashBinPos - vec2(1.0f, 3.0f), m_ThrashBinSize, 0xff00000);

		if (m_Graph->m_SelectedNode)
			m_Graph->drawNodeProps();

		m_BackButton->Render();

		m_Graph->Update();
	}

}

bool Editor::onEvent(Event& ev)
{

	if (m_GraphType == GraphType::None)
	{
		if (ev.getType() == EventType::MousePressed)
		{
			auto mp = static_cast<MousePressedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (m_NeoButton->onMousePressed(mousePos))
				return true;

			else if (m_OrientatButton->onMousePressed(mousePos))
				return true;

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
				{
					m_Graph->m_SelectedNode->m_isMoved = false;

					vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));
					if (mousePos.x > m_ThrashBinPos.x && mousePos.x < m_ThrashBinPos.x + m_ThrashBinSize.x &&
						mousePos.y > m_ThrashBinPos.y && mousePos.y < m_ThrashBinPos.y + m_ThrashBinSize.y)
					{
						m_Graph->deleteNode();
						return true;
					}
				}
			}
		}

		else if (ev.getType() == EventType::MousePressed)
		{
			auto mp = static_cast<MousePressedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			if (m_BackButton->onMousePressed(mousePos))
				return true;
		}

		else if (ev.getType() == EventType::MouseMoved)
		{
			auto mp = static_cast<MouseMovedEvent&>(ev);
			vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

			m_BackButton->onMouseMoved(mousePos);
		}

		m_Graph->onEvent(ev);
	}

	return false;
}

void Editor::createTextBox()
{
	m_TipsBox = std::make_shared<TextBox>(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f),
		(m_Graph->m_MatrixPosition.x + 395.0f) - (m_Graph->m_MatrixPosition.x + 7 * 7.0f + 2) - 2,
		m_LineOffset - 3);
	m_TipsBox->setPosition({ m_Graph->m_MatrixPosition.x + 7 * 7.0f + 4, m_BufferDim.y - m_LineOffset + 2 });
	m_TipsBox->setOffset(7, 0);
	setTip();
}

void Editor::setTip()
{
	m_TipsBox->setText(m_Tips[m_GraphType][m_CurrentTip], Top | Left, { 0.0f, 0.0f, 0.0f });
	m_TipsBox->setText(std::to_string(m_CurrentTip + 1) + "\\" + std::to_string(m_Tips[m_GraphType].size()), Bottom | Right, {0.0f, 0.0f, 0.0f}, false);
}

void Editor::loadTips(const std::string& filepath)
{
	std::fstream doc(filepath);
	if (!doc.good())
		return;
	
	std::string line;
	std::string def;

	GraphType currentType;

	while (std::getline(doc, line))
	{
		if (line.find("Orientat") != std::string::npos)
		{
			if (!def.empty())
				m_Tips[currentType].push_back(def);
			def.clear();
			currentType = GraphType::Oriented;
		}
		else if (line.find("Neorientat") != std::string::npos) {
			if (!def.empty())
				m_Tips[currentType].push_back(def);
			def.clear();
			currentType = GraphType::Unoriented;
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
