#pragma once

#include "Graphics/Texture.h"
#include "Maths/Maths.h"
#include "TextBox.h"

enum CharacterType
{
	Char   = (1 << 0), 
	Digit  = (1 << 1)
};

class InputBox
{
public:
	InputBox(const vec3& color, float width, float height);

	bool onMousePressed(const vec2& pos);
	bool onKeyDown(int code);

	void Render();

	void setPosition(const vec2& position) { m_TextBox->setPosition(position); }

	void setTextSize(int size) { m_TextBox->setTextSize(size); }
	void setTextAnchor(int flag) { m_Flags = flag; }
	void setText(const std::string& text) { m_TextBox->setText(text, m_Flags, m_TextBox->m_TextLayout.m_Color); m_Buffer = text; }
	void setCharacterLimit(int l) { m_CharacterLimit = l; }
	void setCharacterType(CharacterType type) { m_ChType = int(type); }


	const std::string& getBuffer() const { return m_Buffer; }
	int getBufferSize() { return m_TextBox->m_Text.size(); }

private:
	bool m_WaitForInput = false;
	std::string m_Buffer = "";

	int m_Flags = Center;
	int m_ChType = Char | Digit;

	int m_CharacterLimit = 1000;

	std::shared_ptr<TextBox> m_TextBox;
};