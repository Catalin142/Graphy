#pragma once
#include "Texture.h"
#include "Core/Types.h"

class Font
{
	friend class Renderer;

public:
	static void Init(const std::string& path, const std::string& numbers, const std::string& symbols, int glyphWidth, int glyphHeight);

	static uint getGlyphWidth(char c);
	static uint getGlyphHeight() { return m_GlyphHeight; }
	static uint getSymbolOffset(char s);
	static uint getTextWidth(const std::string& text);
	static uint getTextWidth(const std::string& text, int size);

	static bool Exists(char c);

private:
	static std::shared_ptr<Texture> m_FontSheet;
	static std::shared_ptr<Texture> m_NumbersSheet;
	static std::shared_ptr<Texture> m_SymbolsSheet;

	static uint m_GlyphHeight;
	static uint m_GlyphWidth;

	static std::unordered_map<char, int> m_GlyphSize;

private:
	Font() = default;
	~Font() = default;
};