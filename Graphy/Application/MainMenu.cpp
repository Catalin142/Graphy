#include "Core/GrPch.h"
#include "MainMenu.h"
#include "TreeEditor.h"
#include "GraphEditor.h"
#include "TheoryTab.h"

void MainMenu::onAttach()
{
    Subscribe<MousePressedEvent>();
    Subscribe<MouseMovedEvent>();
    Subscribe<KeyPressedEvent>();

    m_Logo = TextureManager::loadTexture("Resources/Menu/Logo.spr");
    m_ButtonFrame = TextureManager::loadTexture("Resources/Menu/ButtonFrame.spr");

    m_Mascota = std::make_shared<Animation>(0.4f); 
    m_Mascota->addFrame("Resources/Menu/Mascota.spr");
    m_Mascota->addFrame("Resources/Menu/Mascota2.spr");

    Renderer::setClearColor(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f);

    m_LogoPosition = vec2(30.0f, 150.0f);
    m_MascotaPosition = m_LogoPosition;
    m_MascotaPosition.x += m_Logo->getWidth() * m_SpritePixelSize.x + 20.0f;

    m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };
    m_NumeleMeuPos = m_BufferDim;
    m_NumeleMeuPos.y = 0;
    m_NumeleMeuPos.x -= Font::getTextWidth("Popa Catalin", 2) + 10.0f;

    GUIManager.Add("Graf", new Button(m_ButtonFrame, vec2(85.0f, 140.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Graf")->setText("Creeaza graf", Center);
    GUIManager.Get<Button>("Graf")->setCallback([]() -> void {
        Application::Get()->setLayer(new TreeEditor);
        });

    GUIManager.Add("Grafic", new Button(m_ButtonFrame, vec2(85.0f, 100.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Grafic")->setText("Creeaza grafic", Center);
    GUIManager.Get<Button>("Grafic")->setCallback([]() -> void {
        Application::Get()->setLayer(new GraphEditor);
        });


    GUIManager.Add("Teorie", new Button(m_ButtonFrame, vec2(85.0f, 60.0f), vec2(200.0f, 30.0f)));
    GUIManager.Get<Button>("Teorie")->setText("Teorie", Center);
    GUIManager.Get<Button>("Teorie")->setCallback([]() -> void {
        Application::Get()->setLayer(new TheoryTab);
        });
}

void MainMenu::onUpdate(float deltaTime)
{
    m_Mascota->Play(deltaTime);

    Renderer::Clear();

    Renderer::renderTexture(m_Logo, m_LogoPosition, m_SpritePixelSize);
    m_Mascota->Render(m_MascotaPosition, m_SpritePixelSize);

    Renderer::drawText("Popa Catalin", m_NumeleMeuPos, 2.0f, vec3(0.0f, 0.0f, 0.0f));
    Renderer::drawText("V 1.0", { 3.0f, 0.0f }, 2.0f, vec3(0.0f, 0.0f, 0.0f));

    GUIManager.Render();
}

bool MainMenu::onEvent(Event& ev)
{
    if (GUIManager.onEvent(ev))
        return true;
}