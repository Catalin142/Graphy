#pragma once

enum EventType
{
	MousePressed,
	MouseMoved,
	MouseReleased,
	MouseScroll,

	KeyPressed,
};

/*
Clasa de Event (interfata) nu o fac pure virtual ca nu mi place sa fac pure virtualuri numa cand chiar e nevoie
*/

class Event
{
	friend class EventDispatcher;

public:
	Event(EventType type) : m_Type(type) {}
	virtual ~Event() = default;

	EventType getType() { return m_Type; }

private:
	EventType m_Type;
	bool m_Handled = false;
};

// Tine toate informatiile despre mouse (pozitie, buton) atunci cand il apas.
class MousePressedEvent : public Event
{
public:
	MousePressedEvent(int code, int x, int y) : m_MouseCode(code), m_MouseX(x), m_MouseY(y), Event(EventType::MousePressed) {}

	static EventType getStaticType() { return EventType::MousePressed; }

	int getX() { return m_MouseX; }
	int getY() { return m_MouseY; }

	int getMouseCode() { return m_MouseCode; }

private:
	int m_MouseX;
	int m_MouseY;

	int m_MouseCode;
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(int x, int y) : m_MouseX(x), m_MouseY(y), Event(EventType::MouseMoved) {}

	static EventType getStaticType() { return EventType::MouseMoved; }

	int getX() { return m_MouseX; }
	int getY() { return m_MouseY; }

private:
	int m_MouseX;
	int m_MouseY;
};

class MouseReleasedEvent : public Event
{
public:
	MouseReleasedEvent(int code, int x, int y) : m_MouseCode(code), m_MouseX(x), m_MouseY(y), Event(EventType::MouseReleased) {}

	static EventType getStaticType() { return EventType::MouseReleased; }

	int getX() { return m_MouseX; }
	int getY() { return m_MouseY; }

	int getMouseCode() { return m_MouseCode; }

private:
	int m_MouseX;
	int m_MouseY;

	int m_MouseCode;
};

class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent(int dy): m_DY(dy), Event(EventType::MouseScroll) {}

	static EventType getStaticType() { return EventType::MouseScroll; }

	int getDelta() { return m_DY; }

private:
	int m_DY;
};

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(int keycode) : m_KeyCode(keycode), Event(EventType::KeyPressed) {}

	static EventType getStaticType() { return EventType::KeyPressed; }
	int getKeyCode() { return m_KeyCode; }

private:
	int m_KeyCode;
};