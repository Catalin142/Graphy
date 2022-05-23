#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../../Graphy/Tree.h"
#include "System/Input.h"
#include "../Manager.h"

class TreeEditor : public Layer
{
public:
	void onAttach() override;
	void onUpdate(float deltaTime) override;
	bool onEvent(Event& ev) override;

	void setTree(const std::string& name) { m_Graph = TreeManager::Get()[name]; m_Graph->recalculateLinks(); m_TreeName = name; }

private:
	std::shared_ptr<Tree> m_Graph;
	vec2 m_MatrixPosition;

	int m_LineOffset = 55.0f;

	bool m_SpaceState = true;
	bool m_RState = true;

	int m_CurrentTip = 0;
	float m_ChangeInterval = 7.0f;
	float m_LastChange = 0.0f;

	std::string m_TreeName = "";
	std::string m_NodeName = "";

	bool m_GUIInitialized = false;
	bool m_TreeExists = false;
	
private:
	void InitializeGUI();
	void setTip();

	void drawNodeProps();
	void drawInfo();
};