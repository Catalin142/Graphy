#pragma once

#include "Core/Layer.h"
#include "GUI/GUIElements/Button.h"
#include "TreeEditor.h"

class TheoryTab : public Layer
{
	friend class TreeEditor;

public:
	void onAttach() override;
	void onUpdate(float deltaTime) override;
	bool onEvent(Event& ev) override;

	static void loadTheory(const std::string & theory);

private:
	GraphType m_SelectedType = GraphType::Oriented;
	int m_Pages = 0;
	int m_CurrentPage = 0;
	int m_ItemsOnPage = 4;

	static std::unordered_map<GraphType, std::vector<std::string>> m_Theory;
	
private:
	void Update();
	void ChangePage(int i);
};
