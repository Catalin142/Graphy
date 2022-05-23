#pragma once

#include "Event/Event.h"
#include "GUI/GUIElements/TextBox.h"
#include "GUI/GUIElements/InputBox.h"
#include "GUI/GUIElements/ButtonPanel.h"
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
	~Tree() = default;

	void addNode(int x, int y);
	void Render();
	bool isLink(int x, int y);

	NodeEvent onEvent(Event& ev);

private:
	std::vector<std::shared_ptr<Node>> m_Nodes;
	std::shared_ptr<Node> m_SelectedNode = nullptr;

	int m_Matrix[MAX_NODES][MAX_NODES]{ 0 };
	int m_DistanceMatrix[MAX_NODES][MAX_NODES]{ 0 };

	int m_LastNumber = 1;

	bool m_Select = false;

	GraphType m_Type;
	vec2 m_BufferDim;

	std::shared_ptr<Texture> m_DeleteSign;

	std::vector<Link> m_Links;
	int m_DrawLink = -1;

	std::array<bool, Modes::Last> m_Modes = { false };

	// Algorithms
	static std::shared_ptr<DijsktraAlgorihm> m_Dijsktra;
	int m_SourceNode = -1;
	int m_DestinationNode = -1;


private:
	std::shared_ptr<Node>& getNode(int id);

	void recalculateGrades();
	void recalculateLinks();

	void deleteNode();
	void refreshAlgorithms();
	int getNodeID(const std::string& name);
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

private:
	Tree* m_Tree;
	int m_Size;

	int m_Source;
	int m_Destination;

	std::vector<int> m_Distances;
	std::vector<bool> m_Visited;
	std::vector<int> m_Parents;

	std::vector<std::pair<int, int>> m_Solution;


private:
	int getMinDistance();
};