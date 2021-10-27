#pragma once

#include "GUI/GUIBase/GUIBaseElement.h"

#include "Graphics/Texture.h"
#include "Maths/Maths.h"
#include "TextBox.h"

enum CharacterType
{
	Char   = (1 << 0), 
	Digit  = (1 << 1)
};

class InputBox : public GUIBaseElement
{
public:
	InputBox(std::string& buffer, const vec3& color, float width, float height);

	void Activate(bool l);

	void Render() override;

	bool onMousePressed(int x, int y) override;
	bool onKeyDown(int code) override;

	void setPosition(const vec2& position) { m_TextBox->setPosition(position); }

	void setTextSize(int size) override { m_TextBox->setTextSize(size); }
	void setText(const std::string& text, TextAnchorFlags flags) override { m_TextBox->setText(text, flags, m_TextBox->m_TextLayout.m_Color); m_Buffer = text; }

	void setTextAnchor(int flag) { m_Flags = flag; }
	void setCharacterLimit(int l) { m_CharacterLimit = l; }
	void setCharacterType(int type) { m_CharacterType = type; }
	void setDefaultText(const std::string& tx) { m_DefaultText = tx; m_TextBox->setText(m_DefaultText, m_Flags, m_TextBox->m_TextLayout.m_Color); }

	const std::string& getBuffer() const { return m_Buffer; }
	int getBufferSize() { return m_TextBox->m_Text.size(); }

	bool isActive() { return m_WaitForInput; }

private:
	bool m_WaitForInput = false;
	std::string& m_Buffer;
	std::string m_DefaultText = "";

	int m_Flags = Center;
	int m_CharacterType = Char | Digit;

	int m_CharacterLimit = 1000;

	std::shared_ptr<TextBox> m_TextBox;
};