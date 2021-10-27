#pragma once

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"
#include "../../Graphy/Tree.h"
#include "System/Input.h"
#include "GUI/GUIElements/Button.h"
#include "Graphics/Animation.h"
#include "GUI/GUIElements/InputBox.h"
#include "GUI/GUIElements/ButtonPanel.h"

class MainMenu : public Layer
{
public:
	~MainMenu() = default;

	void onAttach() override;
	void onUpdate(float deltaTime) override;
	bool onEvent(Event& ev) override;

private:
	std::shared_ptr<Texture> m_Logo;
	std::shared_ptr<Animation> m_Mascota;
	std::shared_ptr<Texture> m_ButtonFrame;

	std::shared_ptr<ButtonPanel> m_ButtonPanel;

	vec2 m_LogoPosition;
	vec2 m_MascotaPosition;
	vec2 m_SpritePixelSize = vec2(6.0f, 6.0f);

	vec2 m_BufferDim;
	vec2 m_NumeleMeuPos;

	int m_Index = 0;
	bool m_SelectionMenu = false;
	bool m_TabState = true;

	std::string m_SelectedItem;

private:
	void Refresh();
};