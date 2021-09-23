#include "Core/GrPch.h"
#include "Layer.h"

Layer::~Layer()
{
	EventDispatcher::removeListener(this);
}
