#include "Core/GrPch.h"
#include "Node.h"
#include "Graph.h"

#include "System/Input.h"
#include "Core/Application.h"
#include "Graphics/Font.h"
#include "Renderer/Renderer.h"

Graph::Graph(GraphType type) : m_Type(type)
{
	m_BufferDim.x = Application::Get()->getBuffer()->getWidth();
	m_BufferDim.y = Application::Get()->getBuffer()->getHeight();
	m_MatrixPosition.x = 5.0f;
	m_MatrixPosition.y = m_BufferDim.y - Font::getGlyphHeight() - 3.0f;

	m_InputBox = std::make_shared<InputBox>(vec3(0.6f, 0.6f, 0.6f), 18, 7);
	m_InputBox->setCharacterLimit(3);
	m_InputBox->setCharacterType(Digit);

	m_DeleteSign = std::make_shared<Texture>("Resources/Editor/Delete.spr");

	m_Links.reserve((40 * (40 - 1)) / 2);
}

void Graph::addNode(int x, int y)
{
	if (m_Nodes.empty())
		m_LastNumber = 0;

	if (m_Nodes.size() < 40)
	{
		y = Clamp(y, 0, m_BufferDim.y - 70.0f);
		x = Clamp(x, 15.0f, m_BufferDim.x - 15.0f);
		std::shared_ptr<Node> node = std::make_shared<Node>(m_Nodes.size(), std::to_string(++m_LastNumber), vec2(x, y), 15.0f);
		m_Nodes.push_back(node);
	}
}

void Graph::Update()
{
	for (int x = 0; x < m_Nodes.size(); x++)
	{
		for (int y = 0; y < m_Nodes.size(); y++)
		{
			// Sa deseneze 2 linii doar daca e orientat daca nu doar o linie
			bool go = true;
			if (x > y && m_Type == GraphType::Unoriented)
				go = false;

			if (m_Matrix[x][y] == 1 && go)
			{
				auto node = getNode(x);
				auto snode = getNode(y);

				float rot = std::atan2(snode->m_Position.y - node->m_Position.y, snode->m_Position.x - node->m_Position.x);
				vec2 dir = vec2(cos(rot), sin(rot));
				vec2 linePosBeg = node->m_Position + dir * node->m_Radius;
				vec2 linePosEnd = snode->m_Position - dir * snode->m_Radius;

				Renderer::drawLine(linePosBeg, linePosEnd, 2.0f, { 0.0f, 0.0f, 0.0f });

				if (m_Type == GraphType::Oriented)
				{
					rot -= degToRad(30.0f);
					Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), 2.0f, { 0.0f, 0.0f, 0.0f });

					rot += degToRad(60.0f);
					Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), 2.0f, { 0.0f, 0.0f, 0.0f });
				}
			}
		}
	}

	if (m_DrawLink != -1)
	{
		float size = m_Links[m_DrawLink].m_BoxSize;
		Renderer::renderTexture(m_DeleteSign, m_Links[m_DrawLink].m_Middle, { 2.0f, 2.0f });
	}

	for (const auto& node : m_Nodes)
		node->Render();

	drawMatrix();

	if (m_SelectedNode)
		if (Input::isPressed('R') && m_SelectedNode->m_isHovered)
		{
			deleteNode();
		}
}

bool Graph::onEvent(Event& ev)
{
	if (ev.getType() == EventType::MousePressed)
	{
		auto mp = static_cast<MousePressedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		if (m_InputBox->onMousePressed(mousePos))
			return true;

		for (auto& it = m_Nodes.rbegin(); it != m_Nodes.rend(); it++)
		{
			auto node = *it;
			if (node->isPressed(mousePos.x, mousePos.y))
			{
				if (!m_Select)
				{
					node->m_isMoved = true;
				}
				else
				{
					if (m_SelectedNode->m_ID != node->m_ID)
					{
						m_Matrix[m_SelectedNode->m_ID][node->m_ID] = 1;

						if (m_Type == GraphType::Unoriented)
							m_Matrix[node->m_ID][m_SelectedNode->m_ID] = 1;

						recalculateGrades();
						recalculateLinks();
					}
				}
				return true;
			}
		}

		if (m_DrawLink != -1)
		{
			float size = m_Links[m_DrawLink].m_BoxSize + 7.0f;
			vec2 boxPos = m_Links[m_DrawLink].m_Middle - vec2(3.5f, 3.5f);

			if (mousePos.x > boxPos.x && mousePos.x < boxPos.x + size &&
				mousePos.y > boxPos.y && mousePos.y < boxPos.y + size)
			{
				m_Matrix[m_Links[m_DrawLink].m_IDLeft][m_Links[m_DrawLink].m_IDRight] = 0;
				if (m_Type == GraphType::Unoriented)
				{
					m_Matrix[m_Links[m_DrawLink].m_IDRight][m_Links[m_DrawLink].m_IDLeft] = 0;
				}

				m_DrawLink = -1;
				recalculateLinks();
				recalculateGrades();

				return true;
			}
		}
	}

	else if (ev.getType() == EventType::MouseMoved)
	{
		auto mp = static_cast<MouseMovedEvent&>(ev);
		auto pos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		bool isMoving = false;
		if (m_SelectedNode)
		{
			if (m_SelectedNode->m_isMoved == true)
			{
				isMoving = true;
				pos.y = Clamp(pos.y, 0, Application::Get()->getBuffer()->getHeight() - 70.0f);
				pos.x = Clamp(pos.x, 0, m_BufferDim.x);
				m_SelectedNode->SetPosition(pos);
				m_SelectedNode->Refresh();

				std::for_each(m_Links.begin(), m_Links.end(), [&](auto& link) {
					link.getMiddle();
					});
			}
		}

		for (auto& it = m_Nodes.rbegin(); it != m_Nodes.rend(); it++)
		{
			auto& node = *it;
			if (!isMoving)
				if (node->isHovered(pos.x, pos.y) && !m_Select)
				{
					m_SelectedNode = node;
					break;
				}
		}

		bool draw = false;

		for (int i = m_Links.size() - 1; i >= 0; i--)
		{
			float size = m_Links[i].m_BoxSize + 7.0f;
			vec2 boxPos = m_Links[i].m_Middle - vec2(3.5f, 3.5f);

			if (pos.x > boxPos.x && pos.x < boxPos.x + size &&
				pos.y > boxPos.y && pos.y < boxPos.y + size)
			{
				draw = true;
				m_DrawLink = i;
				break;
			}
		}

		if (!draw)
			m_DrawLink = -1;
	}

	else if (ev.getType() == EventType::KeyPressed)
	{
		auto ke = static_cast<KeyPressedEvent&>(ev).getKeyCode();
		if (m_InputBox->onKeyDown(ke))
		{
			m_SelectedNode->m_Name = m_InputBox->getBuffer();
			m_SelectedNode->Refresh();
			return true;
		}
	}
	return false;
}

std::shared_ptr<Node>& Graph::getNode(int id)
{
	auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](const auto& node) -> bool {
		return node->m_ID == id; });
	return *it;
}

void Graph::drawMatrix()
{
	int Limit;
	if (Input::isPressed(VK_TAB))
	{
		Limit = m_Nodes.size();
		Renderer::drawQuad({ m_MatrixPosition.x - 1.0f, m_MatrixPosition.y - (Limit - 1) * 7.0f }, { Limit * 7.0f, Limit * 7.0f }, { 1.0f, 1.0f, 1.0f });
	}

	else Limit = Clamp(m_Nodes.size(), 0, 7);

	auto pos = m_MatrixPosition;
	for (int x = 0; x < Limit; x++)
	{
		pos.x = m_MatrixPosition.x;
		for (int y = 0; y < Limit; y++)
		{
			Renderer::drawNumber((char)m_Matrix[x][y] + '0', pos, 1, 0x000000);
			pos.x += 7.0f;
		}
		pos.y -= Font::getGlyphHeight();
	}
}

void Graph::drawNodeProps()
{
	auto pos = m_MatrixPosition;
	pos.x += 400.0f;

	m_InputBox->setPosition(pos + vec2(Font::getTextWidth("Nod: ") + 1.0f, 0.0f));
	m_InputBox->setText(m_SelectedNode->m_Name);
	m_InputBox->Render();

	Renderer::drawText("Nod: ", pos, 1, {0.0f, 0.0f, 0.0f});
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("ID [in matrice]: " + std::to_string(m_SelectedNode->m_ID), pos, 1, {0.0f, 0.0f, 0.0f});
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad interior: " + std::to_string(m_SelectedNode->m_InternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();

	Renderer::drawText("Grad exterior: " + std::to_string(m_SelectedNode->m_ExternalDegree), pos, 1, { 0.0f, 0.0f, 0.0f });
	pos.y -= Font::getGlyphHeight();
}

void Graph::deleteNode()
{
	int ID = m_SelectedNode->m_ID;
	int size = m_Nodes.size();

	for (int i = 0; i < size; i++)
	{
		for (int j = ID; j < size; j++)
		{
			if (j != size - 1)
				m_Matrix[i][j] = m_Matrix[i][j + 1];
			else
				m_Matrix[i][j] = 0;
		}
	}

	for (int i = ID; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i != size - 1)
				m_Matrix[i][j] = m_Matrix[i + 1][j];
			else
				m_Matrix[i][j] = 0;
		}
	}

	auto& delNode = std::find(m_Nodes.begin(), m_Nodes.end(), m_SelectedNode);
	for (auto& it = m_Nodes.end() - 1; it != delNode; it--)
		it->get()->m_ID = (it - 1)->get()->m_ID;

	m_Nodes.erase(delNode);

	for (auto& node : m_Nodes)
		node->Refresh();

	recalculateLinks();
	recalculateGrades();
	
	m_SelectedNode = nullptr;
}

void Graph::recalculateGrades()
{
	if (m_Nodes.size() > 0)
	{
		for (auto& node : m_Nodes)
		{
			int id = node->m_ID;

			int ext = 0;
			int inter = 0;

			for (int x = 0; x < m_Nodes.size(); x++)
				ext += m_Matrix[id][x];

			for (int y = 0; y < m_Nodes.size(); y++)
				inter += m_Matrix[y][id];

			node->m_ExternalDegree = ext;
			node->m_InternalDegree = inter;
		}
	}
}

void Graph::recalculateLinks()
{
	m_Links.clear();
	for(int x = 0; x < m_Nodes.size(); x++)
		for (int y = 0; y < m_Nodes.size(); y++)
		{
			if (m_Matrix[x][y] == 1)
			{
				auto& node1 = getNode(x);
				auto& node2 = getNode(y);

				m_Links.emplace_back(node1->m_Position, node2->m_Position, x, y);
			}
		}
}
