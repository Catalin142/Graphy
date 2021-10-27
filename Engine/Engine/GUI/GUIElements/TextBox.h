#pragma once

#include "GUI/GUIBase/GUIBaseElement.h"

#include "Graphics/Texture.h"
#include "Graphics/Font.h"

class TextBox : public GUIBaseElement
{
	friend class InputBox;

public:
	TextBox(const vec3& color, float width, float height);
	~TextBox() = default;

	void Render() override;

	void setPosition(const vec2& pos) { m_Position = pos; }
	void setPosition(int flags);

	void setSize(int width, int height) { m_Width = width; m_Height = height; Refresh(); }
	void setTextSize(int x) override { m_TextLayout.m_TextSize = x; }
	void setTextPosition(int flags) { m_TextLayout.m_Flags = flags; }

	void setText(const std::string& text, int flags, const vec3& color, bool clear = true);
	void setText(const std::string& text, bool clear = true);

	void setOffset(int x, int y) { m_xOffset = x; m_yOffset = y; }

private:
	unsigned long m_Color;

	float m_Width;
	float m_Height;

	vec2 m_Position = { 0.0f, 0.0f };

	std::string m_Text;

	struct TextLayout
	{
		int m_Rows;
		int m_TextWidth;
		int m_Flags;
		vec3 m_Color;
		int m_TextSize = 1;

	} m_TextLayout;

	struct SubString
	{
		std::string m_String;
		vec2 m_Position;
	};

	// daca am mai mult de 1 rand
	std::vector<SubString> m_SubStrings;

	int m_xOffset = 0;
	int m_yOffset = 0;

private:
	void Refresh() {
		setText(m_Text, m_TextLayout.m_Flags, m_TextLayout.m_Color);
	}
};