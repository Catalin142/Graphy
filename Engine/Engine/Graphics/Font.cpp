#include "Core/GrPch.h"
#include "Font.h"

std::shared_ptr<Texture> Font::m_FontSheet;
std::shared_ptr<Texture> Font::m_NumbersSheet;
std::shared_ptr<Texture> Font::m_SymbolsSheet;

uint Font::m_GlyphHeight;
uint Font::m_GlyphWidth;

std::unordered_map<char, int> Font::m_GlyphSize;

/*
Valorile din m_GlyphSize sunt puse manual pentru ca fontul e facut in sprite editor si nu am unelte sa fac asta automat acolo.
Numarul reprezinta cat de lat e fiecare caracter ca toate sa stea la distanta egala unul de altu.
*/

void Font::Init(const std::string& path, const std::string& numbers, const std::string& symbols, int glyphWidth, int glyphHeight)
{
	m_FontSheet = TextureManager::loadTexture(path);
	m_NumbersSheet =  TextureManager::loadTexture(numbers);
	m_SymbolsSheet =  TextureManager::loadTexture(symbols);

	m_GlyphWidth = glyphWidth;
	m_GlyphHeight = glyphHeight;

	m_GlyphSize['a'] = glyphWidth - 2;
	m_GlyphSize['b'] = glyphWidth - 2;
	m_GlyphSize['c'] = glyphWidth - 2;
	m_GlyphSize['d'] = glyphWidth - 2;
	m_GlyphSize['e'] = glyphWidth - 2;
	m_GlyphSize['f'] = glyphWidth - 2;
	m_GlyphSize['g'] = glyphWidth - 2;
	m_GlyphSize['h'] = glyphWidth - 2;
	m_GlyphSize['i'] = glyphWidth - 5;
	m_GlyphSize['j'] = glyphWidth - 2;
	m_GlyphSize['k'] = glyphWidth - 2;
	m_GlyphSize['l'] = glyphWidth - 2;
	m_GlyphSize['m'] = glyphWidth - 1;
	m_GlyphSize['n'] = glyphWidth - 2;
	m_GlyphSize['o'] = glyphWidth - 2;
	m_GlyphSize['p'] = glyphWidth - 2;
	m_GlyphSize['q'] = glyphWidth - 2;
	m_GlyphSize['r'] = glyphWidth - 2;
	m_GlyphSize['s'] = glyphWidth - 2;
	m_GlyphSize['t'] = glyphWidth - 1;
	m_GlyphSize['u'] = glyphWidth - 2;
	m_GlyphSize['v'] = glyphWidth - 1;
	m_GlyphSize['w'] = glyphWidth - 1;
	m_GlyphSize['x'] = glyphWidth - 3;
	m_GlyphSize['y'] = glyphWidth - 2;
	m_GlyphSize['z'] = glyphWidth - 2;
	m_GlyphSize['0'] = glyphWidth - 2;
	m_GlyphSize['1'] = glyphWidth - 4;
	m_GlyphSize['2'] = glyphWidth - 2;
	m_GlyphSize['3'] = glyphWidth - 2;
	m_GlyphSize['4'] = glyphWidth - 2;
	m_GlyphSize['5'] = glyphWidth - 2;
	m_GlyphSize['6'] = glyphWidth - 2;
	m_GlyphSize['7'] = glyphWidth - 2;
	m_GlyphSize['8'] = glyphWidth - 2;
	m_GlyphSize['9'] = glyphWidth - 2;
	m_GlyphSize['!'] = glyphWidth - 5;
	m_GlyphSize['?'] = glyphWidth - 2;
	m_GlyphSize[':'] = glyphWidth - 5;
	m_GlyphSize[';'] = glyphWidth - 4;
	m_GlyphSize['.'] = glyphWidth - 5;
	m_GlyphSize[','] = glyphWidth - 4;
	m_GlyphSize['['] = glyphWidth - 4;
	m_GlyphSize[']'] = glyphWidth - 4;
	m_GlyphSize['='] = glyphWidth - 2;
	m_GlyphSize['*'] = glyphWidth - 5;
	m_GlyphSize['/'] = glyphWidth - 1;
	m_GlyphSize['-'] = glyphWidth - 2;
	m_GlyphSize['+'] = glyphWidth - 1;
	m_GlyphSize['<'] = glyphWidth - 1;
	m_GlyphSize['>'] = glyphWidth - 1;
	m_GlyphSize['\\'] = glyphWidth - 4;
}

uint Font::getGlyphWidth(char c)
{
	if (!(c >= -1 && c <= 255))
	{
		GR_WARNING(c, " nu e disponibil");
		return 0;
	}

	if (std::isupper(c))
		c = tolower(c);

	if (m_GlyphSize.find(c) == m_GlyphSize.end())
		return 0;

	return m_GlyphSize[c];
}

uint Font::getSymbolOffset(char s)
{
	GR_ASSERT(m_GlyphSize.find(s) == m_GlyphSize.end(), s, " nu e disponibil");

	int n = 0;
	switch (s)
	{
		case '!':  n =  0;  break;
		case '?':  n =  1;  break;
		case ':':  n =  2;  break;
		case ';':  n =  3;  break;
		case '.':  n =  4;  break;
		case ',':  n =  5;  break;
		case '[':  n =  6;  break;
		case ']':  n =  7;  break;
		case '=':  n =  8;  break;
		case '*':  n =  9;  break;
		case '/':  n = 10;  break;
		case '-':  n = 11;  break;
		case '+':  n = 12;  break;
		case '<':  n = 13;  break;
		case '>':  n = 14;  break;
		case '\\': n = 15;  break;
	}

	return n;
}

uint Font::getTextWidth(const std::string& text)
{
	if (text.empty())
	{
		GR_WARN("Variabila \"text\" nu are voie sa fie goala");
		return 0;
	}

	uint size = 0;

	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] != ' ')
			size += getGlyphWidth(text[i]);
		else size += 5;
	}

	// pun aici spatiul intre caractere, mai putin la ultimul
	size += text.size() - 1;

	return size;
}

uint Font::getTextWidth(const std::string& text, int fSize)
{
	if (text.empty())
		return 0;

	uint size = 0;

	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] != ' ')
			size += getGlyphWidth(text[i]) * fSize;
		else size += 5;
	}

	size += text.size() - 1;

	return size;
}

bool Font::Exists(char c)
{
	return (m_GlyphSize.find(c) != m_GlyphSize.end());
}