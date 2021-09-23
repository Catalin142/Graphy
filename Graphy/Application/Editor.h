#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../Graphy/Graph.h"
#include "System/Input.h"

class Editor : public Layer
{
public:
	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	virtual bool onEvent(Event& ev) override;

private:
	std::shared_ptr<Graph> m_Graph;

	bool m_SpaceState = true;
};