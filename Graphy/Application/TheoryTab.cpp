#include "Core/GrPch.h"
#include "TheoryTab.h"
#include "MainMenu.h"

std::unordered_map<GraphType, std::vector<std::string>> TheoryTab::m_Theory;

void TheoryTab::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseMovedEvent>();

	std::shared_ptr<Texture> m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

	float startPos = (m_BufferDim.x / 2.0f) - (90.0f * 3) / 2;
	GUIManager.Add("Ori", new Button(m_ButtonFrame, vec2(startPos, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Ori")->setTextSize(1);
	GUIManager.Get<Button>("Ori")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Ori")->setText("Graf orientat", Center);
	GUIManager.Get<Button>("Ori")->setCallback([&]() -> void {
		m_SelectedType = GraphType::Oriented;
		});

	GUIManager.Add("Neo", new Button(m_ButtonFrame, vec2(startPos + 90.0f, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Neo")->setTextSize(1);
	GUIManager.Get<Button>("Neo")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Neo")->setText("Graf neorientat", Center);
	GUIManager.Get<Button>("Neo")->setCallback([&]() -> void {
		m_SelectedType = GraphType::Unoriented;
		});


	GUIManager.Add("Gr", new Button(m_ButtonFrame, vec2(startPos + 180.0f, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Gr")->setTextSize(1);
	GUIManager.Get<Button>("Gr")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Gr")->setText("Grafic", Center);
	GUIManager.Get<Button>("Gr")->setCallback([&]() -> void {
		m_SelectedType = GraphType::Graph;
		});

	GUIManager.Add("Return", new Button(m_ButtonFrame, vec2(5.0f, m_BufferDim.y - 25.0f), vec2(vec2(80.0f, 20.0f))));
	GUIManager.Get<Button>("Return")->setTextSize(1);
	GUIManager.Get<Button>("Return")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Return")->setText("Inapoi", Center);
	GUIManager.Get<Button>("Return")->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});

	GUIManager.Add("MainTextBox", new TextBox(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f), m_BufferDim.x, m_BufferDim.y - 35.0f));
	GUIManager.Get<TextBox>("MainTextBox")->setPosition({ 0.0f, 0.0f });
	GUIManager.Get<TextBox>("MainTextBox")->setOffset(10.0f, 15.0f);
	GUIManager.Get<TextBox>("MainTextBox")->setTextSize(1);
	GUIManager.Get<TextBox>("MainTextBox")->setText(m_Theory[m_SelectedType][0] + "\n\n" + m_Theory[m_SelectedType][1] + "\n\n" + m_Theory[m_SelectedType][2] + "\n\n" + 
		m_Theory[m_SelectedType][3],
		Top | Left, { 0.0f, 0.0f, 0.0f });
}

void TheoryTab::onUpdate(float deltaTime)
{
	Renderer::Clear();

	GUIManager.Render();

	Renderer::drawLine({ 0.0f, m_BufferDim.y - 30.0f }, { m_BufferDim.x, m_BufferDim.y - 30.0f }, 0x000000);
}

bool TheoryTab::onEvent(Event& ev)
{
	if (GUIManager.onEvent(ev))
		return true;
}

void TheoryTab::loadTheory(const std::string& filepath)
{
	std::fstream doc(filepath);
	if (!doc.good())
		return;

	std::string line;
	std::string def;

	GraphType currentType;

	while (std::getline(doc, line))
	{
		if (line.find("Orientat") != std::string::npos)
		{
			if (!def.empty())
				m_Theory[currentType].push_back(def);
			def.clear();
			currentType = GraphType::Oriented;
		}
		else if (line.find("Neorientat") != std::string::npos) {
			if (!def.empty())
				m_Theory[currentType].push_back(def);
			def.clear();
			currentType = GraphType::Unoriented;
		}
		else
		{
			if (line.find("Def:") != std::string::npos)
			{
				if (!def.empty())
					m_Theory[currentType].push_back(def);
				def.clear();
			}
			else
			{
				def += line;
				def += '\n';
			}
		}
	}

	doc.close();
}