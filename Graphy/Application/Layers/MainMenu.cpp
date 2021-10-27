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

    GUIManager.Add("Teorie", new Button(m_ButtonFrame, vec2(85.0f, 80.0f), vec2(200.0f, 30.0f)));
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

        GUIManager.Get<Button>("Load")->Enable(false);
        GUIManager.Get<Button>("Delete")->Enable(false);

        GUIManager.Get<Button>("Graf")->Enable(!m_SelectionMenu);
        GUIManager.Get<Button>("Teorie")->Enable(!m_SelectionMenu);

        m_TabState = true;
        m_Index = 0;
    }
    else if (tabstate == false) m_TabState = false;

    if (m_SelectionMenu)
    {
#if 0
        float posY = 270.0f;
        int i = 0;
        for (const auto& tree : TreeManager::Get())
        {
            int textWidth = Font::getTextWidth(tree.first, 2);
            vec2 Pos = { m_BufferDim.x / 2.0f - textWidth / 2.0f, posY };

            vec3 Color = { 0.0f, 0.0f, 0.0f };
            if (i == m_Index)
                Color = { 1.0f, 0.0f, 0.0f };

            Renderer::plotQuad({ m_BufferDim.x / 2.0f - 155.0f, posY - 3.0f }, { 310.0f, 20.0f }, Color);
            Renderer::drawText(tree.first, Pos, 2, { 1.0f, 0.5f, 0.2f });
            
            posY -= 22.0f;
            i++;
        }

        if (GetAsyncKeyState(VK_UP) & 0x0001)
            m_Index--;
        else if (GetAsyncKeyState(VK_DOWN) & 0x0001)
            m_Index++;

        if (m_Index > TreeManager::Get().size() - 1)
            m_Index = 0;
        else if (m_Index < 0)
            m_Index = TreeManager::Get().size() - 1;

        if (Input::isPressed(VK_RETURN))
        {
            auto editor = new TreeEditor();
            std::string t = "";
            int i = 0;
            for (const auto& tree : TreeManager::Get())
            {
                if (i == m_Index)
                { 
                    t = tree.first;
                    break;
                }
                i++;
            }
            editor->setTree(t);
            Application::Get()->setLayer(editor);
        }
#endif
        m_ButtonPanel->Render();
    }

    else
    {
        m_Mascota->Play(deltaTime);
        Renderer::drawLine({ 0.0f, m_BufferDim.y - 16.0f }, { m_BufferDim.x, m_BufferDim.y - 16.0f }, 0x000000, 2.0f);

        Renderer::renderTexture(m_Logo, m_LogoPosition, m_SpritePixelSize);
        m_Mascota->Render(m_MascotaPosition, m_SpritePixelSize);

        Renderer::drawLine({ 0.0f, 16.0f }, { m_BufferDim.x, 16.0f }, 0x000000, 2.0f);

        Renderer::drawText("Popa Catalin", m_NumeleMeuPos, 2.0f, vec3(0.0f, 0.0f, 0.0f));
        Renderer::drawText("V 1.0", { 3.0f, 0.0f }, 2.0f, vec3(0.0f, 0.0f, 0.0f));
    }
    GUIManager.Render();
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

    };

    for (const auto& gr : TreeManager::Get())
    {
        m_ButtonPanel->addButton({ 0.7f, 0.7f, 0.7f }, gr.first, std::bind(buttoncallback, gr.first));
    }
}