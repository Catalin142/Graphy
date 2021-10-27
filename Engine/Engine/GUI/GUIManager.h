#pragma once
#include "GUIBase/GUIBaseElement.h"

#include "GUIElements/Button.h"
#include "GUIElements/InputBox.h"
#include "GUIElements/TextBox.h"

#include "Event/Event.h"


class GUIM
{
public:
	GUIM() = default;
	~GUIM();

	void Add(const std::string& name, GUIBaseElement* elem);
	void Delete(const std::string& element);
	
	template <typename T>
	T* Get(const std::string& name) 
	{
		return static_cast<T*>(m_GUIElements[name]);
	}

	void Render();
	bool onEvent(Event& ev);

private:
	std::unordered_map<std::string, GUIBaseElement*> m_GUIElements;
};