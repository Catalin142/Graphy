#pragma once

#include "Maths/Maths.h"
#include "System/GraphicsContext.h"
#include "Graphics/Texture.h"

class Renderer
{
	friend class GraphicsContext;

public:
	static void Init(std::shared_ptr<GraphicsContext>& buffer);

	static void setClearColor(float r, float g, float b);
	static void Clear(float r, float g, float b);
	static void Clear();

	static void drawQuad(const vec2& pos, const vec2& size, const vec3& color);
	static void drawQuad(const vec2& pos, const vec2& size, unsigned long color);

	static void plotQuad(const vec2& pos, const vec2& size, const vec3& color, float thickness = 1.0f);
	static void plotQuad(const vec2& pos, const vec2& size, unsigned long color, float thickness = 1.0f);

	static void drawLine(const vec2& begin, const vec2& end, const vec3& color, float thickness = 1.0f);
	static void drawLine(const vec2& begin, const vec2& end, unsigned long color, float thickness = 1.0f);

	static void drawCircle(const vec2& pos, float radius, const vec3& color);
	static void plotCircle(const vec2& pos, float radius, const vec3& color);

	static void drawCircle(const vec2& pos, float radius, unsigned long color);
	static void plotCircle(const vec2& pos, float radius, unsigned long color);

	static void setPixel(const vec2& pos, const vec3& color);
	static void setPixel_s(const vec2& pos, const vec3& color);
	static void setPixel(const vec2& pos, unsigned long hexColor);
	static void setPixel_s(const vec2& pos, unsigned long hexColor);

	static void renderTexture(const std::shared_ptr<Texture>& sprite, const vec2& pos, const vec2& size);
	static void renderTexture(const std::shared_ptr<Texture>& sprite, const vec2& pos, const vec2& size, const vec3& color);

	static bool checkPixel(const vec2& pos);

	// Size se refera la pixel size
	static void drawText(const std::string& text, const vec2& pos, float size, const vec3& color);
	static void drawNumber(char n, const vec2& pos, float size, unsigned int color);

	static void Draw();

private:
	static std::shared_ptr<GraphicsContext> m_Buffer;
	static u32* m_BuffersBuffer; // ;-?
	static unsigned long m_ClearColor;

	/*
	Trebuie sa fac 3 funtii separate pentru ca nu ma pot baza pe codul ascii.
	Imi trebuie codul ascii ca sa vad la ce offset e caracterul
	*/

	static void drawChar_i(char c, vec2& pos, float size, unsigned int color);
	static void drawNumber_i(char n, vec2& pos, float size, unsigned int color);
	static void drawSymbol_i(char s, vec2& pos, float size, unsigned int color); // Internal

};