#pragma once

#include "Event/Event.h"
#include "GUI/GUIElements/TextBox.h"
#include "GUI/GUIElements/InputBox.h"
#include "Graphics/Texture.h"
#include "Link.h"

#define MAX_NODES 40

class Node;

enum class GraphType
{
	None,
	Oriented,
	Unoriented,
};

enum class NodeEvent
{
	None,

	Link,
	Select,
	Delete,
	Moved,
};

enum Modes
{
	Length,
	Dijsktra,

	Last,
};

class DijsktraAlgorihm;
class Tree
{
	friend class TreeEditor;
	friend class TreeManager;
	friend class DijsktraAlgorihm;

	friend void Write(std::ofstream& str, const std::shared_ptr<Tree>& data);

public:
	Tree(GraphType type);
	~Tree();

	void addNode(int x, int y);
	void Render();

	NodeEvent onEvent(Event& ev);

private:
	std::vector<std::shared_ptr<Node>>  m_Nodes;
	std::shared_ptr<Node> m_SelectedNode = nullptr;

	int m_Matrix[MAX_NODES][MAX_NODES]{ 0 };
	int m_DistanceMatrix[MAX_NODES][MAX_NODES]{ 0 };

	int m_LastNumber = 1;
	vec2 m_MatrixPosition;

	bool m_Select = false;

	GraphType m_Type;
	vec2 m_BufferDim;

	std::shared_ptr<Texture> m_DeleteSign;

	std::vector<Link> m_Links;
	int m_DrawLink = -1;

	std::array<bool, Modes::Last> m_Modes = { false };

	// Algorithms
	static std::shared_ptr<DijsktraAlgorihm> m_Dijsktra;

private:
	std::shared_ptr<Node>& getNode(int id);

	void recalculateGrades();
	void recalculateLinks();

	void deleteNode();
	void refreshAlgorithms();
};

class DijsktraAlgorihm
{
	friend class Tree;

public:
	DijsktraAlgorihm()
	{
		m_Distances.resize(MAX_NODES, INT_MAX);
		m_Visited.resize(MAX_NODES, false);
		m_Parents.resize(MAX_NODES, 0);
	}

	void Generate(Tree* tree, int src);

	void getSolution(int dest);
	int getTime() { return m_Timer; }

private:
	Tree* m_Tree;
	int m_Size;

	int m_Source;
	int m_Destination;

	std::vector<int> m_Distances;
	std::vector<bool> m_Visited;
	std::vector<int> m_Parents;

	std::vector<std::pair<int, int>> m_Solution;

	int m_Timer = 0;
	bool m_Initialize = false;

private:
	int getMinDistance();
};