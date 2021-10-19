#pragma once

#include "Application.h"
#include "Event/EventListener.h"
#include "GUI/GUIManager.h"

// Fiecare layer are aceleasi functii si am facut clasa cu functii virtuale ca sa pot sa am mai multe "pagini" (Meniu, Setari...)
class Layer : public EventListener
{
	friend class Application;

public:
	Layer() = default;
	virtual ~Layer();

	virtual void onAttach() { }
	virtual void onDetach() { }
	virtual void onUpdate(float dt) { }

	virtual bool onEvent(Event& event) override { return false; }

private:
	bool m_Active = true;

protected:
	GUIM GUIManager;

public:
	static vec2 m_BufferDim;
};