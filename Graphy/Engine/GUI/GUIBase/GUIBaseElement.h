#pragma once

// Base Element al UI pentru a putea sa le polimorfizez (:-?)
enum TextAnchorFlags
{
	Center = (1 << 0),
	Left = (1 << 1),
	Right = (1 << 2),
	Top = (1 << 3),
	Bottom = (1 << 4),
};

class GUIBaseElement
{
	friend class GUIM;

public:
	GUIBaseElement() = default;
	~GUIBaseElement() = default;

	virtual void Render() { }
	virtual bool onMousePressed(int x, int y) { return false; }
	virtual bool onMouseMoved(int x, int y) { return false; }
	virtual bool onKeyDown(int code) { return false; }

	virtual void setTextSize(int size) { }
	virtual void setText(const std::string& text, TextAnchorFlags flags) { }

	void Hide(bool l) { m_IsHidden = l; }
	void Enable(bool l) { m_isActive = l; m_IsHidden = !l; }

private:
	bool m_IsHidden = false;
	bool m_isActive = true;
};