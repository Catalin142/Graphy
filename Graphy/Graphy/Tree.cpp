#include "Core/GrPch.h"
#include "Node.h"
#include "Tree.h"

#include "System/Input.h"
#include "Core/Application.h"
#include "Graphics/Font.h"
#include "Renderer/Renderer.h"

Tree::Tree(GraphType type) : m_Type(type)
{
	m_BufferDim.x = Application::Get()->getBuffer()->getWidth();
	m_BufferDim.y = Application::Get()->getBuffer()->getHeight();
	m_MatrixPosition.x = 5.0f;
	m_MatrixPosition.y = m_BufferDim.y - Font::getGlyphHeight() - 3.0f;

	//m_InputBox = std::make_shared<InputBox>(vec3(0.6f, 0.6f, 0.6f), 18, 7);
	//m_InputBox->setCharacterLimit(3);
	//m_InputBox->setCharacterType(Digit);

	m_DeleteSign = TextureManager::loadTexture("Resources/Editor/Delete.spr");

	m_Links.reserve((40 * (40 - 1)) / 2);
}

void Tree::addNode(int x, int y)
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

void Tree::Render()
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

				Renderer::drawLine(linePosBeg, linePosEnd, { 0.0f, 0.0f, 0.0f }, 2.0f);

				if (m_Type == GraphType::Oriented)
				{
					rot -= degToRad(30.0f);
					Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), { 0.0f, 0.0f, 0.0f }, 2.0f);

					rot += degToRad(60.0f);
					Renderer::drawLine(linePosEnd, (linePosEnd - vec2(cos(rot), sin(rot)) * 10.0f), { 0.0f, 0.0f, 0.0f }, 2.0f);
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
}

NodeEvent Tree::onEvent(Event& ev)
{
	if (ev.getType() == EventType::MousePressed)
	{
		auto mp = static_cast<MousePressedEvent&>(ev);
		vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		if (mp.getMouseCode() == VK_MOUSE_LEFT)
		{
			for (auto& it = m_Nodes.rbegin(); it != m_Nodes.rend(); it++)
			{
				auto node = *it;
				if (node->isPressed(mousePos.x, mousePos.y))
				{
					if (!m_Select)
					{
						node->m_isMoved = true;
						return NodeEvent::Moved;
					}
					else
					{
						if (m_SelectedNode->m_ID != node->m_ID)
						{
							m_Matrix[m_SelectedNode->m_ID][node->m_ID] = 1;

							if (m_Type == GraphType::Unoriented)
								m_Matrix[node->m_ID][m_SelectedNode->m_ID] = 1;

							m_SelectedNode = node;

							recalculateGrades();
							recalculateLinks();
							return NodeEvent::Link;
						}
					}
				}
			}
		}

		else if (mp.getMouseCode() == VK_MOUSE_RIGHT)
		{
			if (m_SelectedNode && m_SelectedNode->isHovered(mousePos.x, mousePos.y))
			{
				deleteNode();
				return NodeEvent::Delete;
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

					return NodeEvent::None;
				}
			}
		}
	}

	else if (ev.getType() == EventType::MouseMoved)
	{
		auto mp = static_cast<MouseMovedEvent&>(ev);
		auto pos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

		NodeEvent returnType = NodeEvent::None;

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

				returnType = NodeEvent::Moved;
			}
		}

		bool found = false;
		for (auto& it = m_Nodes.rbegin(); it != m_Nodes.rend(); it++)
		{
			auto& node = *it;
			if (!isMoving)
				if (node->isHovered(pos.x, pos.y))
				{
					if (!m_Select && found == false)
					{
						if (m_SelectedNode != node)
						{
							if (m_SelectedNode)
								m_SelectedNode->m_Color = { 0.0f, 0.0f, 0.0f };
							m_SelectedNode = node;
						}

						returnType = NodeEvent::Select;
					}

					if (found == false)
						found = true;
					else node->m_isHovered = false;

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

		return returnType;
	}

	return NodeEvent::None;
}

std::shared_ptr<Node>& Tree::getNode(int id)
{
	auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](const auto& node) -> bool {
		return node->m_ID == id; });
	return *it;
}

void Tree::deleteNode()
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

void Tree::recalculateGrades()
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

void Tree::recalculateLinks()
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
