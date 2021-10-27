#include "Core/GrPch.h"
#include "Renderer.h"
#include "Graphics/Font.h"

std::shared_ptr<GraphicsContext> Renderer::m_Buffer;
uint32_t* Renderer::m_BuffersBuffer; // ;-?
unsigned long Renderer::m_ClearColor = 0x00000000;

void Renderer::Init(std::shared_ptr<GraphicsContext>& buffer)
{
	m_Buffer = buffer;
	m_BuffersBuffer = (u32*)m_Buffer->m_MemoryBuffer;
}

void Renderer::setClearColor(float r, float g, float b)
{
	m_ClearColor = createHex({ r, g, b });
}

void Renderer::Clear(float r, float g, float b)
{
	m_ClearColor = createHex({ r, g, b });
	m_Buffer->Clear(m_ClearColor);
}

void Renderer::Clear()
{
	m_Buffer->Clear(m_ClearColor);
}

void Renderer::setPixel(const vec2& pos, const vec3& color)
{
	if (checkPixel(pos))
		return;

	int pixel = (int)pos.y * m_Buffer->m_Width + (int)pos.x;
	*(m_BuffersBuffer + pixel) = createHex(color.r * 255, color.g * 255, color.b * 255);
}

void Renderer::setPixel(const vec2& pos, unsigned long hexColor)
{
	int pixel = (int)pos.y * m_Buffer->m_Width + (int)pos.x;
	*(m_BuffersBuffer + pixel) = hexColor;
}

void Renderer::drawQuad(const vec2& pos, const vec2& size, unsigned long color)
{
	int startY = Clamp(pos.y, 0, m_Buffer->m_Height - 1.0f);
	int endY = Clamp(pos.y + size.y, 0, m_Buffer->m_Height - 1.0f);

	int startX = Clamp(pos.x, 0, m_Buffer->m_Width - 1.0f);
	int endX = Clamp(pos.x + size.x, 0, m_Buffer->m_Width - 1.0f);

	u32* pixel;
	for (int y = startY; y < endY; y++)
	{
		pixel = (u32*)m_BuffersBuffer + (int)startX + y * m_Buffer->m_Width;
		for (int x = startX; x < endX; x++)
		{
			*(pixel++) = color;
		}
	}
}

void Renderer::drawQuad(const vec2& pos, const vec2& size, const vec3& color)
{
	auto hexColor = createHex(color.r * 255, color.g * 255, color.b * 255);

	drawQuad(pos, size, hexColor);
}

void Renderer::plotQuad(const vec2& pos, const vec2& size, unsigned long color, float thickness)
{
	drawLine(pos, pos + vec2(size.x, 0), color, thickness);
	drawLine(pos, pos + vec2(0, size.y), color, thickness);

	drawLine(pos + vec2(0, size.y), pos + vec2(size.x + 1, size.y), color, thickness);
	drawLine(pos + vec2(size.x, 0), pos + vec2(size.x, size.y), color, thickness);
}

void Renderer::plotQuad(const vec2& pos, const vec2& size, const vec3& color, float thickness)
{
	auto hexColor = createHex(color.r * 255, color.g * 255, color.b * 255);

	plotQuad(pos, size, hexColor, thickness);
}

void Renderer::setPixel_s(const vec2& pos, const vec3& color)
{
	if (checkPixel(pos))
		return;
	setPixel(pos, color);
}

void Renderer::setPixel_s(const vec2& pos, unsigned long hexColor)
{
	if (checkPixel(pos))
		return;
	setPixel(pos, hexColor);
}

void Renderer::renderTexture(const std::shared_ptr<Texture>& sprite, const vec2& pos, const vec2& size)
{
	float posY = pos.y;

	for (int i = 0; i < sprite->m_Height; i++)
	{
		float posX = pos.x;
		for (int j = 0; j < sprite->m_Width; j++)
		{
			if (!(sprite->m_Buffer[i * sprite->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, size, sprite->m_Buffer[i * sprite->m_Width + j]);
			posX += size.x;
		}
		posY += size.y;
	}
}

void Renderer::renderTexture(const std::shared_ptr<Texture>& sprite, const vec2& pos, const vec2& size, const vec3& color)
{
	float posY = pos.y;

	for (int i = 0; i < sprite->m_Height; i++)
	{
		float posX = pos.x;
		for (int j = 0; j < sprite->m_Width; j++)
		{
			if (!(sprite->m_Buffer[i * sprite->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, size, color);

			posX += size.x;
		}
		posY += size.y;
	}
}

bool Renderer::checkPixel(const vec2& pos)
{
	return (pos.x > m_Buffer->m_Width - 1.0f || pos.y > m_Buffer->m_Height - 1.0f || pos.x < 0 || pos.y < 0);
}

// Bresenham alg http://members.chello.at/~easyfilter/bresenham.html
void Renderer::drawLine(const vec2& begin, const vec2& end, const vec3& color, float thickness)
{
	drawLine(begin, end, createHex(color), thickness);
}

void Renderer::drawLine(const vec2& begin, const vec2& end, unsigned long color, float thickness)
{
	vec2 dt = end - begin;
	double length = dt.magnitude();
	vec2 addFactor = dt.normalize() * thickness;

	dt.x = begin.x;
	dt.y = begin.y;

	for (double i = 0; i < length / thickness; i++)
	{
		drawQuad({ dt.x, dt.y }, { thickness,thickness }, color);
		dt.x += addFactor.x;
		dt.y += addFactor.y;
	}
}

void Renderer::drawCircle(const vec2& pos, float radius, const vec3& color)
{
	unsigned long pxColor = createHex(color);
	drawCircle(pos, radius, pxColor);
}

void Renderer::plotCircle(const vec2& pos, float radius, const vec3& color)
{
	unsigned long pxColor = createHex(color);
	plotCircle(pos, radius, pxColor);
}

void Renderer::drawText(const std::string& text, const vec2& pos, float size, const vec3& color)
{
	auto hexColor = createHex(color);
	vec2 Position = pos;

	for (int ch = 0; ch < text.size(); ch++)
	{
		char c = text[ch];
		if (c == ' ')
		{
			Position.x += 5;
			continue;
		}

		// daca e litera mare nu gaseste bine offsetul
		if (std::isupper(c))
			c = std::tolower(c);

		if (std::isdigit(c))
			drawNumber_i(c, Position, size, hexColor);
		else if (c >= 'a' && c <= 'z')
			drawChar_i(c, Position, size, hexColor);
		else if (Font::Exists(c))
			drawSymbol_i(c, Position, size, hexColor);
	}
}

void Renderer::drawNumber(char n, const vec2& Pos, float size, unsigned int color)
{
	int offset = n - '0';

	auto pos = Pos;
	float posY = pos.y;
	float posX = pos.x;

	float glyphWidth = offset * Font::m_GlyphWidth + Font::getGlyphWidth(n) + 1;

	for (int i = 0; i < Font::m_GlyphHeight; i++)
	{
		posX = pos.x;

		for (int j = offset * Font::m_GlyphWidth + 1; j < glyphWidth; j++)
		{
			if (!(Font::m_NumbersSheet->m_Buffer[i * Font::m_NumbersSheet->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, { size, size }, color);
			posX += size;
		}
		posY += size;
	}
	pos.x = posX + 1;
}

void Renderer::drawCircle(const vec2& pos, float radius, unsigned long color)
{
	int x = (int)-radius;
	int y = 0;
	int err = 2 - 2 * (int)radius;

	while (x < 0)
	{
		for (int i = (int)pos.x + x; i < (int)pos.x - x; i++)
		{
			setPixel_s(vec2((float)i, pos.y + y), color);
			setPixel_s(vec2((float)i, pos.y - y), color);
		}

		setPixel_s(vec2(pos.x - x, pos.y + y), color);
		setPixel_s(vec2(pos.x - y, pos.y - x), color);
		setPixel_s(vec2(pos.x + x, pos.y - y), color);
		setPixel_s(vec2(pos.x + y, pos.y + x), color);

		radius = (float)err;

		if (radius <= y) err += ++y * 2 + 1;
		if (radius > x || err > y) err += ++x * 2 + 1;
	}
}

void Renderer::plotCircle(const vec2& pos, float radius, unsigned long color)
{
	int x = (int)-radius;
	int y = 0;
	int err = 2 - 2 * (int)radius;

	while (x < 0)
	{
		setPixel_s(vec2(pos.x - x, pos.y + y), color);
		setPixel_s(vec2(pos.x - y, pos.y - x), color);
		setPixel_s(vec2(pos.x + x, pos.y - y), color);
		setPixel_s(vec2(pos.x + y, pos.y + x), color);

		radius = (float)err;

		if (radius <= y) err += ++y * 2 + 1;
		if (radius > x || err > y) err += ++x * 2 + 1;
	}
}

void Renderer::drawChar_i(char c, vec2& pos, float size, unsigned int hexColor)
{
	int offset = c - 'a';

	float posY = pos.y;
	float posX = pos.x;

	int width = Font::getGlyphWidth(c);

	for (int i = 0; i < Font::m_GlyphHeight; i++)
	{
		posX = pos.x;

		for (int j = offset * Font::m_GlyphWidth + 1; j < offset * Font::m_GlyphWidth + width + 1; j++)
		{
			if (!(Font::m_FontSheet->m_Buffer[i * Font::m_FontSheet->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, { size, size }, hexColor);
			posX += size;
		}
		posY += size;
	}
	pos.x = posX + 1;
}

void Renderer::drawNumber_i(char n, vec2& pos, float size, unsigned int hexColor)
{
	int offset = n - '0';

	float posY = pos.y;
	float posX = pos.x;

	int width = Font::getGlyphWidth(n);

	for (int i = 0; i < Font::m_GlyphHeight; i++)
	{
		posX = pos.x;

		for (int j = offset * Font::m_GlyphWidth + 1; j < offset * Font::m_GlyphWidth + width + 1; j++)
		{
			if (!(Font::m_NumbersSheet->m_Buffer[i * Font::m_NumbersSheet->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, { size, size }, hexColor);
			posX += size;
		}
		posY += size;
	}
	pos.x = posX + 1;
}

void Renderer::drawSymbol_i(char s, vec2& pos, float size, unsigned int hexColor)
{
	int offset = Font::getSymbolOffset(s);

	float posY = pos.y;
	float posX = pos.x;

	int width = Font::getGlyphWidth(s);
	for (int i = 0; i < Font::m_GlyphHeight; i++)
	{
		posX = pos.x;

		for (int j = offset * Font::m_GlyphWidth + 1; j < offset * Font::m_GlyphWidth + width + 1; j++)
		{
			if (!(Font::m_SymbolsSheet->m_Buffer[i * Font::m_SymbolsSheet->m_Width + j] == -1))
				Renderer::drawQuad({ posX, posY }, { size, size }, hexColor);
			posX += size;
		}
		posY += size;
	}
	pos.x = posX + 1;
}

void Renderer::Draw()
{
	m_Buffer->Render();
}
