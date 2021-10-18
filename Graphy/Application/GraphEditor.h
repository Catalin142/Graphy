#pragma once

#include "../Graphy/Node.h"
#include "Maths/Maths.h"
#include "Core/Layer.h"
#include "GUI/GUIElements/Button.h"

class GraphEditor : public Layer
{
public:
	void onAttach() override;
	void onUpdate(float deltaTime) override;
	bool onEvent(Event& ev) override;

private:
	std::vector<std::shared_ptr<Node>> m_Points;

	int m_LineOffset = 55.0f - 38.0f;
	float m_LineDistance = 19.0f;
	float m_Margin = 12.0f;

	std::shared_ptr<Node> m_CurrentPoint;
	std::string m_Letter = "a";

	bool m_SpaceState = true;
};