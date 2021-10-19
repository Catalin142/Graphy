#pragma once
#include "Event.h"
#include "EventDispatcher.h"

/*
Asta e clasa principala care raspune la eventuri.
Folosesc un bitset (practic un vector de booluri) ca sa verific la ce eventuri reactioneaza
*/

class EventListener
{
	friend class EventDispatcher;

public:
	EventListener() { EventDispatcher::addListener(this); }
	virtual ~EventListener() = default;

	// Aici se face verificarea si isi da handle evenimentul (e referinta pt (adi neacsu) ca ma folosesc de polimorfism, daca nu trebuie sa fie pointer alocat cu new si era mai lent).
	virtual bool onEvent(Event& event) = 0;

	template<typename T>
	void Subscribe()
	{
		bool f = std::is_base_of<Event, T>::value;
		GR_ASSERT(!f, "Trebuie sa fie event");
		m_Following[static_cast<int>(T::getStaticType())] = 1;
	}

	template <typename T>
	void Unsubscribe()
	{
		m_Following[T::getStaticType()] = 0;
	}

private:
	// Ma bazez pe faptul ca fiecare event are un ID intreg(int) < 12.
	std::bitset<12> m_Following;
	bool m_HandlesEvents = true;
};