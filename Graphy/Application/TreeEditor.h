#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../Graphy/Tree.h"
#include "System/Input.h"

class TreeEditor : public Layer
{
public:
	void onAttach() override;
	void onUpdate(float deltaTime) override;
	bool onEvent(Event& ev) override;

private:
	std::shared_ptr<Tree> m_Graph;
	GraphType m_GraphType = GraphType::None;

	int m_LineOffset = 55.0f;

	bool m_SpaceState = true;

	int m_CurrentTip = 0;
	float m_ChangeInterval = 7.0f;
	float m_LastChange = 0.0f;
	
private:
	void InitializeGUI();
	void setTip();

	void drawNodeProps();
	void drawMatrix();
};