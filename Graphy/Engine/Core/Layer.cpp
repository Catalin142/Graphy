#include "Core/GrPch.h"
#include "Layer.h"

vec2 Layer::m_BufferDim;

Layer::~Layer()
{
	EventDispatcher::removeListener(this);
}
