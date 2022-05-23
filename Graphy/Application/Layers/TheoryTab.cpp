#include "Core/GrPch.h"
#include "TheoryTab.h"
#include "MainMenu.h"

std::unordered_map<GraphType, std::vector<std::string>> TheoryTab::m_Theory;

void TheoryTab::onAttach()
{
	Subscribe<MousePressedEvent>();
	Subscribe<MouseMovedEvent>();

	std::shared_ptr<Texture> m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

	float startPos = (m_BufferDim.x / 2.0f) - (90.0f * 2) / 2;
	GUIManager.Add("Ori", new Button(m_ButtonFrame, vec2(startPos, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Ori")->setTextSize(1);
	GUIManager.Get<Button>("Ori")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Ori")->setText("Graf orientat", Center);
	GUIManager.Get<Button>("Ori")->setCallback([&]() -> void {
		m_SelectedType = GraphType::Oriented; Update();
		}); 

	GUIManager.Add("Neo", new Button(m_ButtonFrame, vec2(startPos + 90.0f, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Neo")->setTextSize(1);
	GUIManager.Get<Button>("Neo")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Neo")->setText("Graf neorientat", Center);
	GUIManager.Get<Button>("Neo")->setCallback([&]() -> void {
		m_SelectedType = GraphType::Unoriented; Update();
		});

	GUIManager.Add("Return", new Button(m_ButtonFrame, vec2(5.0f, m_BufferDim.y - 25.0f), vec2(80.0f, 20.0f)));
	GUIManager.Get<Button>("Return")->setTextSize(1);
	GUIManager.Get<Button>("Return")->setHoverAnimationDist(3);
	GUIManager.Get<Button>("Return")->setText("Inapoi", Center);
	GUIManager.Get<Button>("Return")->setCallback([&]() -> void {
		Application::Get()->setLayer(new MainMenu);
		});

	GUIManager.Add("MainTextBox", new TextBox(vec3(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f), m_BufferDim.x, m_BufferDim.y - 85.0f));
	GUIManager.Get<TextBox>("MainTextBox")->setPosition({ 0.0f, 50.0f });
	GUIManager.Get<TextBox>("MainTextBox")->setOffset(10.0f, 15.0f);
	GUIManager.Get<TextBox>("MainTextBox")->setTextSize(1);
	GUIManager.Get<TextBox>("MainTextBox")->setTextPosition(Left | Top);

	int xpos = m_BufferDim.x / 2.0f;

	GUIManager.Add("PageLeft", new Button({ 225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f }, vec2(xpos - 10.0f - 7.0f, 1), vec2(7.0f, 7.0f)));
	GUIManager.Get<Button>("PageLeft")->setTextSize(1);
	GUIManager.Get<Button>("PageLeft")->setHoverAnimationDist(0);
	GUIManager.Get<Button>("PageLeft")->setText("<", Center);
	GUIManager.Get<Button>("PageLeft")->setCallback([&]() -> void {
		ChangePage(-1);
		});

	GUIManager.Add("PageRight", new Button({ 225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f }, vec2(xpos + 10.0f, 1), vec2(7.0f, 7.0f)));
	GUIManager.Get<Button>("PageRight")->setTextSize(1);
	GUIManager.Get<Button>("PageRight")->setHoverAnimationDist(0);
	GUIManager.Get<Button>("PageRight")->setText(">", Center);
	GUIManager.Get<Button>("PageRight")->setCallback([&]() -> void {
		ChangePage(1);
		});

	Update();
}

void TheoryTab::onUpdate(float deltaTime)
{
	Renderer::Clear();
	Renderer::drawLine({ 0.0f, m_BufferDim.y - 30.0f }, { m_BufferDim.x, m_BufferDim.y - 30.0f }, 0x000000);

	std::string text = std::to_string(m_CurrentPage + 1) + "\\" + std::to_string(m_Pages);
	int size = Font::getTextWidth(text);
	Renderer::drawText(text, { m_BufferDim.x / 2.0f - size / 2.0f, 1.0f }, 1, { 0.0f, 0.0f, 0.0f });

	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		ChangePage(-1);

	else if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		ChangePage(1);
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

void TheoryTab::Update()
{
	m_Pages = std::ceil(m_Theory[m_SelectedType].size() / (float)m_ItemsOnPage);
	m_CurrentPage = 0;
	ChangePage(0);
}

void TheoryTab::ChangePage(int i)
{
	m_CurrentPage += i;
	if (m_CurrentPage < 0)
		m_CurrentPage = m_Pages - 1;
	else if (m_CurrentPage > m_Pages - 1)
		m_CurrentPage = 0;

	std::string info;
	for (int i = m_CurrentPage * m_ItemsOnPage; i < std::min(m_CurrentPage * m_ItemsOnPage + m_ItemsOnPage, (int)m_Theory[m_SelectedType].size()); i++)
	{
		info += m_Theory[m_SelectedType][i] + "\n\n\n";
	}

	GUIManager.Get<TextBox>("MainTextBox")->setText(info);
}
