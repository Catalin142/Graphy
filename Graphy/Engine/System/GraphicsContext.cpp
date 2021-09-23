#include "Core/GrPch.h"

#include "GraphicsContext.h"

GraphicsContext::GraphicsContext(std::shared_ptr<Window>& window, u32 width, u32 height) : m_Window(window)
{
	m_Width = width;
	m_Height = height;

	setViewport(m_Width, m_Height);

	m_BitMapInfo.bmiHeader.biSize = sizeof(m_BitMapInfo.bmiHeader);
	m_BitMapInfo.bmiHeader.biPlanes = 1;
	m_BitMapInfo.bmiHeader.biBitCount = sizeof(u32) * 8;
	m_BitMapInfo.bmiHeader.biCompression = BI_RGB;
}

GraphicsContext::~GraphicsContext()
{
	VirtualFree(m_MemoryBuffer, 0, MEM_FREE);
}

void GraphicsContext::setViewport(int width, int height)
{
	m_Width = width;
	m_Height = height;

	if (m_MemoryBuffer)
		VirtualFree(m_MemoryBuffer, 0, MEM_RELEASE);

	m_MemoryBuffer = VirtualAlloc(0, m_Width * m_Height * sizeof(u32), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	m_BitMapInfo.bmiHeader.biWidth = (LONG)m_Width;
	m_BitMapInfo.bmiHeader.biHeight = (LONG)m_Height;

	m_BufferSize = u32(m_Width * m_Height);
}

void GraphicsContext::Clear(unsigned long hex)
{
	u32* first = (u32*)m_MemoryBuffer;

	for (uint i = 0; i < m_BufferSize; i++)
		*(first++) = hex;
}

void GraphicsContext::Clear(float r, float g, float b)
{
	Clear(createHex(int(r * 255), int(g * 255), int(b * 255)));
}

void GraphicsContext::Render()
{
	StretchDIBits(m_Window->getDeviceContext(), 0, 0, m_Window->getWidth(), m_Window->getHeight(), 0, 0, m_Width, m_Height,
		m_MemoryBuffer, &m_BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
}