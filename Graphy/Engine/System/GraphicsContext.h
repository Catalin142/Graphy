#pragma once
#include "Window.h"

/*
Tot ce desenez pe ecran e desenat intr-un buffer in memorie de latime * inaltime * sizeof(u32). (sizeof(u32) = 4 bytes (R G B -) * 8 = 32 biti)
Dupa folosesc un bitmapinfo ca sa dau proprietati bufferului de bitmap si-l trimit la desenat.
*/

class GraphicsContext
{
	friend class Renderer;

public:
	GraphicsContext(std::shared_ptr<Window>& window, u32 width, u32 height);
	~GraphicsContext();

	void setViewport(int width, int height);

	void Clear(unsigned long hex);
	void Clear(float r, float g, float b);

	void* getBuffer() { return m_MemoryBuffer; }
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }
	float getAspectRatio() { return (float)m_Width / (float)m_Height; }

	void Render();

private:
	void* m_MemoryBuffer = nullptr;

	BITMAPINFO m_BitMapInfo;

	std::shared_ptr<Window> m_Window;
	u32 m_BufferSize;

	u32 m_Width;
	u32 m_Height;
};