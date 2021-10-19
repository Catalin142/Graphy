#pragma once
#include "Event.h"

class EventListener;
/*
Asta trimite eventurile la listeneri.
*/

class EventDispatcher
{
public:
	static void addListener(EventListener* l);
	static void removeListener(EventListener* l);

	static void Dispatch(Event& event);
	static void clearListeners();

private:
	static std::vector<EventListener*> m_Listeners;

private:
	EventDispatcher() = default;
	~EventDispatcher() = default;
};