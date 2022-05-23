#include "Core/GrPch.h"
#include "MainMenu.h"
#include "TreeEditor.h"
#include "TheoryTab.h"
#include "../Manager.h"

void MainMenu::onAttach()
{
    Subscribe<MousePressedEvent>();
    Subscribe<MouseMovedEvent>();
    Subscribe<KeyPressedEvent>();
    Subscribe<MouseScrollEvent>();

    m_Logo = TextureManager::loadTexture("Resources/Menu/Logo.spr");
    m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

    m_Mascota = std::make_shared<Animation>(0.4f); 
    m_Mascota->addFrame("Resources/Menu/Mascota.spr");
    m_Mascota->addFrame("Resources/Menu/Mascota2.spr");

    Renderer::setClearColor(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f);

    m_LogoPosition = vec2(40.0f, 130.0f);
    m_MascotaPosition = m_LogoPosition;
    m_MascotaPosition.x += m_Logo->getWidth() * m_SpritePixelSize.x + 45.0f;
    m_MascotaPosition.y = 120.0f;

    m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };
    m_NumeleMeuPos = m_BufferDim;
    m_NumeleMeuPos.y = 0;
    m_NumeleMeuPos.x -= Font::getTextWidth("Popa Catalin", 2) + 10.0f;

    GUIManager.Add("Graf", new Button(m_ButtonFrame, vec2(85.0f, 120.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Graf")->setText("Creeaza graf", Center);
    GUIManager.Get<Button>("Graf")->setCallback([]() -> void {
        Application::Get()->setLayer(new TreeEditor);
        });

    GUIManager.Add("Exit", new Button(m_ButtonFrame, vec2(470.0f, 30.0f), vec2(80.0f, 20.0f)));
    GUIManager.Get<Button>("Exit")->setText("Iesi", Center);
    GUIManager.Get<Button>("Exit")->setCallback([]() -> void {
        Application::Get()->Close();
        });

    GUIManager.Add("Tab", new Button(m_ButtonFrame, vec2(85.0f, 80.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Tab")->setText("Deschide graf", Center);
    GUIManager.Get<Button>("Tab")->setCallback([&]() -> void {
        m_SelectionMenu = true;
        ChangeTab();
        });

    GUIManager.Add("Teorie", new Button(m_ButtonFrame, vec2(85.0f, 40.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Teorie")->setText("Teorie", Center);
    GUIManager.Get<Button>("Teorie")->setCallback([]() -> void {
        Application::Get()->setLayer(new TheoryTab);
        });

    GUIManager.Add("Delete", new Button(vec3(0.7f, 0.7f, 0.7f), vec2(370.0f, 165.0f), vec2(120.0f, 25.0f)));
    GUIManager.Get<Button>("Delete")->setText("Sterge", Center);
    GUIManager.Get<Button>("Delete")->setHoverAnimationDist(1.0f);
    GUIManager.Get<Button>("Delete")->setBorderColor({ 0.0f, 0.0f, 0.0f });
    GUIManager.Get<Button>("Delete")->setCallback([&]() -> void {
        if (!m_SelectedItem.empty())
        {
            TreeManager::deleteTree(m_SelectedItem);
            m_ButtonPanel->Clear();
            m_ButtonPanel->Refresh();
            Refresh();

            m_SelectedItem.clear();

            GUIManager.Get<Button>("Load")->Enable(false);
            GUIManager.Get<Button>("Delete")->Enable(false);
        }
        });
    GUIManager.Get<Button>("Delete")->Enable(false);

    GUIManager.Add("Load", new Button(vec3(0.7f, 0.7f, 0.7f), vec2(370.0f, 200.0f), vec2(120.0f, 25.0f)));
    GUIManager.Get<Button>("Load")->setText("Incarca", Center);
    GUIManager.Get<Button>("Load")->setHoverAnimationDist(1.0f);
    GUIManager.Get<Button>("Load")->setBorderColor({ 0.0f, 0.0f, 0.0f });
    GUIManager.Get<Button>("Load")->setCallback([&]() -> void {
        if (!m_SelectedItem.empty())
        {
            auto editor = new TreeEditor();
            editor->setTree(m_SelectedItem);
            Application::Get()->setLayer(editor);
        }
        });
    GUIManager.Get<Button>("Load")->Enable(false);

    GUIManager.Add("Back", new Button(vec3(0.7f, 0.7f, 0.7f), vec2(5.0f, m_BufferDim.y - 13.0f), vec2(45.0f, 8.0f)));
    GUIManager.Get<Button>("Back")->setTextSize(1);
    GUIManager.Get<Button>("Back")->setText("Inapoi", Center);
    GUIManager.Get<Button>("Back")->setHoverAnimationDist(1.0f);
    GUIManager.Get<Button>("Back")->setBorderColor({ 0.0f, 0.0f, 0.0f });
    GUIManager.Get<Button>("Back")->setCallback([&]() -> void {
        m_SelectionMenu = false;
        ChangeTab();
        });
    GUIManager.Get<Button>("Back")->Enable(false);

    m_ButtonPanel = std::make_shared<ButtonPanel>(vec3(1.0f, 1.0f, 1.0f), vec2(100.0f, 10.0f), vec2(200.0f, 300.0f));
    m_ButtonPanel->setButtonSize({ 180.0f, 20.0f });

    Refresh();
}

void MainMenu::onUpdate(float deltaTime)
{
    Renderer::Clear();

    bool tabstate = Input::isPressed(VK_TAB);
    if (tabstate != m_TabState && tabstate == true)
    {
        m_SelectionMenu = !m_SelectionMenu;

        ChangeTab();
        m_TabState = true;
    }
    else if (tabstate == false) m_TabState = false;

    if (m_SelectionMenu)
        m_ButtonPanel->Render();

    else
    {
        m_Mascota->Play(deltaTime);
        Renderer::drawLine({ 0.0f, m_BufferDim.y - 16.0f }, { m_BufferDim.x, m_BufferDim.y - 16.0f }, 0x000000, 2.0f);

        Renderer::renderTexture(m_Logo, m_LogoPosition, m_SpritePixelSize);
        m_Mascota->Render(m_MascotaPosition, m_SpritePixelSize);

        Renderer::drawLine({ 0.0f, 16.0f }, { m_BufferDim.x, 16.0f }, 0x000000, 2.0f);
        Renderer::drawText("Popa Catalin", m_NumeleMeuPos, 2.0f, vec3(0.0f, 0.0f, 0.0f));
        Renderer::drawText("V 1.3", { 3.0f, 0.0f }, 2.0f, vec3(0.0f, 0.0f, 0.0f));
    }
}

bool MainMenu::onEvent(Event& ev)
{
    if (GUIManager.onEvent(ev))
        return true;

    else if (m_SelectionMenu)
    {
        if (m_ButtonPanel->onEvent(ev))
            return true;
        else
        {
            if (ev.getType() == EventType::MousePressed)
            {
                GUIManager.Get<Button>("Load")->Enable(false);
                GUIManager.Get<Button>("Delete")->Enable(false);
            }
        }
    }

    return false;
}

void MainMenu::Refresh()
{
    auto buttoncallback = [&](const std::string& name) {
        m_SelectedItem = name;

        GUIManager.Get<Button>("Load")->Enable(true);
        GUIManager.Get<Button>("Delete")->Enable(true);
        GUIManager.Get<Button>("Back")->Enable(true);

    };

    for (const auto& gr : TreeManager::Get())
    {
        m_ButtonPanel->addButton({ 0.7f, 0.7f, 0.7f }, gr.first, std::bind(buttoncallback, gr.first));
    }
}

void MainMenu::ChangeTab()
{
    m_ButtonPanel->Refresh();

    GUIManager.Get<Button>("Load")->Enable(m_SelectionMenu);
    GUIManager.Get<Button>("Delete")->Enable(m_SelectionMenu);
    GUIManager.Get<Button>("Back")->Enable(m_SelectionMenu);

    GUIManager.Get<Button>("Graf")->Enable(!m_SelectionMenu);
    GUIManager.Get<Button>("Teorie")->Enable(!m_SelectionMenu);
    GUIManager.Get<Button>("Tab")->Enable(!m_SelectionMenu);
    GUIManager.Get<Button>("Exit")->Enable(!m_SelectionMenu);

    m_Index = 0;
}
