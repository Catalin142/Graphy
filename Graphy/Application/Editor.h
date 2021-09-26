#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../Graphy/Graph.h"
#include "System/Input.h"
#include "UI/Button.h"
#include "UI/TextBox.h"

class Editor : public Layer
{
public:
	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	virtual bool onEvent(Event& ev) override;

private:
	std::shared_ptr<Graph> m_Graph;
	GraphType m_GraphType = GraphType::None;

	int m_LineOffset = 55.0f;

	std::shared_ptr<TextBox> m_TipsBox;

	std::shared_ptr<Button> m_OrientatButton;
	std::shared_ptr<Button> m_NeoButton;
	std::shared_ptr<Button> m_BackButton;

	std::shared_ptr<Texture> m_ThrashBin;
	vec2 m_ThrashBinSize;
	vec2 m_ThrashBinPos;

	vec2 m_BufferDim;

	bool m_SpaceState = true;

	std::unordered_map<GraphType, std::vector<std::string>> m_Tips;
	int m_CurrentTip = 0;
	float m_ChangeInterval = 7.0f;
	float m_LastChange = 0.0f;
	
private:
	void createTextBox();
	void setTip();
	void loadTips(const std::string& filepath);
};