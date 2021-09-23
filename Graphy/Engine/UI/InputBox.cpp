#include "Core/GrPch.h"
#include "InputBox.h"
#include "System/Input.h"

InputBox::InputBox(const vec3& color, float width, float height) : m_TextBox(std::make_shared<TextBox>(color, width, height)) { }

bool InputBox::onMousePressed(const vec2& pos)
{
	bool pressed = (m_TextBox->m_Position.x + m_TextBox->m_Width > pos.x && m_TextBox->m_Position.x < pos.x &&
		m_TextBox->m_Position.y + m_TextBox->m_Height > pos.y && m_TextBox->m_Position.y < pos.y);

	if (pressed)
		m_WaitForInput = true;
	else m_WaitForInput = false;

	return pressed;
}

bool InputBox::onKeyDown(int code)
{
	if (m_WaitForInput == true)
	{
		Input::blockInput(true);
		auto c = (char)code;
		if (code == VK_BACK && !m_Buffer.empty())
			m_Buffer.pop_back();

		else if (code == VK_RETURN)
			m_WaitForInput = false;

		else if (c >= -1 && c <= 255 && m_Buffer.size() < m_CharacterLimit)
		{
			if (m_ChType & Digit)
			{
				if (std::isdigit(c))
					m_Buffer += c;
			}

			if (m_ChType & Char)
			{
				if (!std::isdigit(c))
					m_Buffer += c;
			}
		}

		m_TextBox->setText(m_Buffer, m_Flags, m_TextBox->m_TextLayout.m_Color );
		return true;

	}
	return false;
}

void InputBox::Render()
{
	m_TextBox->Render();
}
