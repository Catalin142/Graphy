#pragma once

#include "Event/Event.h"
#include "UI/TextBox.h"
#include "UI/InputBox.h"
#include "Graphics/Texture.h"
#include "Link.h"

class Node;

enum class TreeType
{
	None,
	Oriented,
	Unoriented
};

class Tree
{
	friend class TreeEditor;

public:
	Tree(TreeType type);
	~Tree() = default;

	void addNode(int x, int y);
	void Render();

	bool onEvent(Event& ev);

private:
	std::vector<std::shared_ptr<Node>>  m_Nodes;
	std::shared_ptr<Node> m_SelectedNode = nullptr;

	std::shared_ptr<InputBox> m_InputBox;

	int m_Matrix[40][40]{ 0 };
	int m_LastNumber = 1;
	vec2 m_MatrixPosition;

	bool m_Select = false;

	TreeType m_Type;
	vec2 m_BufferDim;

	std::shared_ptr<Texture> m_DeleteSign;

	std::vector<Link> m_Links;
	int m_DrawLink = -1;

private:
	std::shared_ptr<Node>& getNode(int id);

	void recalculateGrades();
	void recalculateLinks();

	void deleteNode();
};