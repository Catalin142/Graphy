#pragma once

#include "../../Graphy/Node.h";
#include "../../Graphy/Tree.h";

static void Write(std::ofstream& str, const std::shared_ptr<Node>& data)
{
	str << data->m_ID << " " << data->m_Name << " " << data->m_Position.x << " " << data->m_Position.y << std::endl;
}

static void Write(std::ofstream& str, const std::shared_ptr<Tree>& data)
{
	str << (int)data->m_Type << std::endl;

	int limit = data->m_Nodes.size();
	str << limit << std::endl;

	for (const auto& node : data->m_Nodes)
		Write(str, node);

	for (int i = 0; i < limit; i++)
	{
		for (int y = 0; y < limit; y++)
		{
			str << data->m_Matrix[i][y] << " ";
		}
		str << std::endl;
	}
}

static void SerializeTree(std::ofstream& out, const std::shared_ptr<Tree>& tree, const std::string& name)
{
	out << name << std::endl;
	Write(out, tree);
}