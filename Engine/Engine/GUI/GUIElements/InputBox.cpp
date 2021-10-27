#include "Core/GrPch.h"
#include "InputBox.h"
#include "System/Input.h"

InputBox::InputBox(std::string& buffer, const vec3& color, float width, float height) : m_TextBox(std::make_shared<TextBox>(color, width, height)), m_Buffer(buffer) { }

bool InputBox::onMousePressed(int x, int y)
{
	bool pressed = (m_TextBox->m_Position.x + m_TextBox->m_Width > x && m_TextBox->m_Position.x < x &&
		m_TextBox->m_Position.y + m_TextBox->m_Height > y && m_TextBox->m_Position.y < y);

	if (pressed)
	{
		m_TextBox->setText(m_Buffer, m_Flags, m_TextBox->m_TextLayout.m_Color);
		m_WaitForInput = true;
	}
	else
	{
		m_WaitForInput = false;
		if (m_Buffer.empty())
			m_TextBox->setText(m_DefaultText, m_Flags, m_TextBox->m_TextLayout.m_Color);
	}

	return pressed;
}

void InputBox::Activate(bool l)
{
	m_WaitForInput = l;
}

bool InputBox::onKeyDown(int code)
{
	if (m_WaitForInput == true)
	{
		//Input::blockInput(true);
		auto c = (char)code;
		if (code == VK_BACK && !m_Buffer.empty())
		{
			m_Buffer.pop_back();
		}

		else if (code == VK_RETURN || code == VK_ESCAPE)
			m_WaitForInput = false;

		else if (c >= -1 && c <= 255 && m_Buffer.size() < m_CharacterLimit)
		{
			if (m_CharacterType & Digit)
			{
				if (std::isdigit(c))
					m_Buffer += c; 
			}

			if (m_CharacterType & Char)
			{
				if (!std::isdigit(c) && c != ' ')
					m_Buffer += c;
			}

		}

		m_TextBox->setText(m_Buffer, m_Flags, m_TextBox->m_TextLayout.m_Color);
		return true;
	}
	else Input::blockInput(false);

	return false;
}

void InputBox::Render()
{
	m_TextBox->Render();
}
