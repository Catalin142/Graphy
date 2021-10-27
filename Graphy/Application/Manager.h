#pragma once
#include "../Graphy/Tree.h"

class TreeManager
{
public:
	static void Serialize(const std::string& filepath);
	static void Deserialize(const std::string& path);

	static void saveTree(const std::string& name, const std::shared_ptr<Tree>& tree);
	static void deleteTree(const std::string& name);

	static std::shared_ptr<Tree>& getTree(const std::string& name);
	static std::unordered_map<std::string, std::shared_ptr<Tree>>& Get();

private:
	static std::unordered_map<std::string, std::shared_ptr<Tree>> m_Trees;

private:
	TreeManager() = default;
	~TreeManager() = default;

};