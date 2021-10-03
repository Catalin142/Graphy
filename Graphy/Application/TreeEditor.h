#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../Graphy/Tree.h"
#include "System/Input.h"
#include "UI/Button.h"
#include "UI/TextBox.h"

class TreeEditor : public Layer
{
public:
	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	virtual bool onEvent(Event& ev) override;

private:
	std::shared_ptr<Tree> m_Graph;
	TreeType m_GraphType = TreeType::None;

	int m_LineOffset = 55.0f;

	std::shared_ptr<TextBox> m_TipsBox;

	std::shared_ptr<Button> m_OrientatButton;
	std::shared_ptr<Button> m_NeoButton;
	std::shared_ptr<Button> m_BackButton;

	vec2 m_BufferDim;

	bool m_SpaceState = true;

	std::unordered_map<TreeType, std::vector<std::string>> m_Tips;
	int m_CurrentTip = 0;
	float m_ChangeInterval = 7.0f;
	float m_LastChange = 0.0f;
	
private:
	void createTextBox();
	void setTip();
	void loadTips(const std::string& filepath);

	void drawNodeProps();
	void drawMatrix();
};