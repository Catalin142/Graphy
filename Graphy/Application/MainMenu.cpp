#include "Core/GrPch.h"
#include "MainMenu.h"
#include "Editor.h"

void MainMenu::onAttach()
{
    Subscribe<MousePressedEvent>();
    Subscribe<MouseMovedEvent>();
    Subscribe<KeyPressedEvent>();

    m_Logo = std::make_shared<Texture>("Resources/Menu/Logo.spr");
    m_ButtonFrame = std::make_shared<Texture>("Resources/Menu/ButtonFrame.spr");

    m_Mascota = std::make_shared<Animation>(0.4f); 
    m_Mascota->addFrame("Resources/Menu/Mascota.spr");
    m_Mascota->addFrame("Resources/Menu/Mascota2.spr");

    Renderer::setClearColor(225.0f / 255.0f, 227.0f / 255.0f, 221.0f / 255.0f);

    m_LogoPosition = vec2(30.0f, 150.0f);
    m_MascotaPosition = m_LogoPosition;
    m_MascotaPosition.x += m_Logo->getWidth() * m_SpritePixelSize.x + 20.0f;

    m_BufferDim = { (float)Application::Get()->getBuffer()->getWidth(), (float)Application::Get()->getBuffer()->getHeight() };
    m_NumeleLuSefuPos = m_BufferDim;
    m_NumeleLuSefuPos.y = 0;
    m_NumeleLuSefuPos.x -= Font::getTextWidth("Popa Catalin", 2) + 10.0f;

    m_GraphButton = std::make_shared<Button>(m_ButtonFrame, vec2(85.0f, 140.0f), vec2(200.0f, 30.0f));
    m_GraphButton->setText("Creeaza graf", Center);
    m_GraphButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
    m_GraphButton->setCallback([]() -> void {
        Application::Get()->setLayer(new Editor);
        });

    m_TutoialButton = std::make_shared<Button>(m_ButtonFrame, vec2(85.0f, 100.0f), vec2(200.0f, 30.0f));
    m_TutoialButton->setText("Teorie", Center);
    m_TutoialButton->TextColor = vec3(0.0f, 0.0f, 0.0f);
    m_TutoialButton->setCallback([]() -> void {
        std::cout << "A" << std::endl;
        });
}

void MainMenu::onUpdate(float deltaTime)
{
    m_Mascota->Play(deltaTime);

    Renderer::Clear();

    Renderer::renderTexture(m_Logo, m_LogoPosition, m_SpritePixelSize);
    m_Mascota->Render(m_MascotaPosition, m_SpritePixelSize);

    Renderer::drawText("Popa Catalin", m_NumeleLuSefuPos, 2.0f, vec3(0.0f, 0.0f, 0.0f));
    Renderer::drawText("V 1.0", { 3.0f, 0.0f }, 2.0f, vec3(0.0f, 0.0f, 0.0f));

    m_GraphButton->Render();
    m_TutoialButton->Render();
}

bool MainMenu::onEvent(Event& ev)
{
    if (ev.getType() == EventType::MousePressed)
    {
        auto mp = static_cast<MousePressedEvent&>(ev);
        vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

        if (m_GraphButton->onMousePressed(mousePos))
            return true;

        else if (m_TutoialButton->onMousePressed(mousePos))
            return true;

        return false;
    }

    else if (ev.getType() == EventType::MouseMoved)
    {
        auto mp = static_cast<MouseMovedEvent&>(ev);
        vec2 mousePos = Input::WindowToBufferCoordonates(vec2(mp.getX(), mp.getY()));

        if (m_GraphButton->onMouseMoved(mousePos))
            return true;

        else  if (m_TutoialButton->onMouseMoved(mousePos))
            return true;

        return false;
    }

    /*else if (ev.getType() == EventType::KeyPressed)
    {
        if (m_InputBox->onKeyDown(static_cast<KeyPressedEvent&>(ev).getKeyCode()))
            return true;
    }*/

}