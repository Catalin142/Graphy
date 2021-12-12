#pragma once
#include <string>

#include "Maths/Maths.h"

// Singurele texturi suportate sunt cele facut in FlaCat SpriteEditor.
/*
In fisierul (.spr):
width height si dupa width * height x unsigned long (culori in hex), pentru transparent e -1 
*/

class Texture
{
	friend class Renderer;
	friend class TextBox;
	friend class InteractableUI;
	friend class TextureManager;

public:
	Texture() = default;
	Texture(const std::string& filepath);

	~Texture();

	const unsigned long& getPixelColor(float x, float y) const;

	float getAspectRatio() { return (float)m_Width / (float)m_Height; }
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }

private:
	unsigned long* m_Buffer = nullptr;

	int m_Width;
	int m_Height;

	std::string m_Filepath;

private:

	bool loadTexture(const std::string& filepath);
};

class TextureManager
{
public:
	static std::shared_ptr<Texture> loadTexture(const std::string& filepath);

private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_TextureCache;
};