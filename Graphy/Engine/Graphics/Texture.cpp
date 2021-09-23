#include "Core/GrPch.h"
#include "Texture.h"

#include "Maths/Maths.h"

Texture::Texture(const std::string& filepath) : m_Filepath(filepath)
{
	assert(loadSprite(filepath));
}

bool Texture::loadSprite(const std::string& filepath)
{
	std::ifstream readFile(m_Filepath.c_str(), std::ios::in | std::ios::binary);
	std::string extension = std::string(m_Filepath.begin() + m_Filepath.find_last_of(".") + 1, m_Filepath.end());

	if (!readFile.good())
		return false;

	if (extension != "spr")
		return false;

	readFile.read((char*)&m_Width, sizeof(m_Width));
	readFile.read((char*)&m_Height, sizeof(m_Height));

	if (m_Buffer != nullptr)
		delete[] m_Buffer;

	m_Buffer = new unsigned long[m_Width * m_Height];
	long temp = 0;
	for (int i = 0; i < m_Width * m_Height; i++)
	{
		readFile.read((char*)&temp, sizeof(long));
		m_Buffer[i] = temp;
	}

	return true;
}

const unsigned long& Texture::getPixelColor(float x, float y) const
{
	if (x < 0) x = -x;
	if (y < 0) y = -y;

	float xCoord = x * m_Width;
	float yCoord = y * m_Height;

	return *(m_Buffer + (int)yCoord * m_Width + (int)xCoord);
}
