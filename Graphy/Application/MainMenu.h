#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../Graphy/Graph.h"
#include "System/Input.h"
#include "UI/Button.h"
#include "Graphics/Animation.h"
#include "UI/InputBox.h"

class MainMenu : public Layer
{
public:
	~MainMenu() = default;

	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	virtual bool onEvent(Event& ev) override;


private:
	std::shared_ptr<Texture> m_Logo;
	std::shared_ptr<Animation> m_Mascota;
	std::shared_ptr<Texture> m_ButtonFrame;

	std::shared_ptr<Button> m_GraphButton;
	std::shared_ptr<Button> m_TutoialButton;

	vec2 m_LogoPosition;
	vec2 m_MascotaPosition;
	vec2 m_SpritePixelSize = vec2(6.0f, 6.0f);

	vec2 m_BufferDim;
	vec2 m_NumeleLuSefuPos;
};