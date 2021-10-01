#pragma once

#include "../Graphy/Node.h"
#include "Maths/Maths.h"
#include "Core/Layer.h"

class GraphEditor : public Layer
{
public:
	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	virtual bool onEvent(Event& ev) override;

private:
	struct Point
	{
		std::shared_ptr<Node> Node;
		vec2 m_CartezianCoord;
	};

	std::vector<Point> m_Points;
	vec2 m_BufferDim;

	int m_LineOffset = 55.0f;
	float m_LineDistance = 19.0f;
	float m_Margin = 12.0f;
};