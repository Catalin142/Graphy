#include "Core/GrPch.h"
#include "Manager.h"
#include "Tools/TreeSerializer.h"
#include "../Graphy/Tree.h"

std::unordered_map<std::string, std::shared_ptr<Tree>> TreeManager::m_Trees;

void TreeManager::saveTree(const std::string& name, const std::shared_ptr<Tree>& tree)
{
	if (m_Trees.find(name) == m_Trees.end())
		m_Trees.insert(std::pair(name, tree));
	Serialize("graf.gf");
}

void TreeManager::deleteTree(const std::string& name)
{
	if (m_Trees.find(name) != m_Trees.end())
		m_Trees.erase(name);
}

std::shared_ptr<Tree>& TreeManager::getTree(const std::string& name)
{
	if (m_Trees.find(name) != m_Trees.end())
		return m_Trees[name];
}

std::unordered_map<std::string, std::shared_ptr<Tree>>& TreeManager::Get()
{
	return m_Trees;
}

void TreeManager::Serialize(const std::string& filepath)
{
	std::ofstream out(filepath);
	
	out << m_Trees.size() << std::endl;
	if (out.good())
	{
		for (const auto& tree : m_Trees)
			SerializeTree(out, tree.second, tree.first);
	}
}

void TreeManager::Deserialize(const std::string& path)
{
	std::ifstream file(path);
	if (!file.good())
		return;

	int n;
	file >> n;

	for (int i = 0; i < n; i++)
	{
		std::string name;
		file >> name;

		int grType;
		file >> grType;

		std::shared_ptr<Tree> newTree = std::make_shared<Tree>(GraphType(grType));
		
		int nodes;
		file >> nodes;

		for (int i = 0; i < nodes; i++)
		{
			int id, name;
			float posx, posy;
			file >> id >> name >> posx >> posy;
			newTree->m_Nodes.push_back(std::make_shared<Node>(id, std::to_string(name), vec2(posx, posy), 15.0f));
		}

		for(int i = 0; i < nodes; i++)
			for (int j = 0; j < nodes; j++)
			{
				file >> newTree->m_Matrix[i][j];
			}

		m_Trees[name] = newTree;
	}

	file.close();
}