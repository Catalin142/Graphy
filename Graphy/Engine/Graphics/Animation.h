//TODO: Animatie pentru iepure !
#include "Graphics/Texture.h"

class Animation
{
public:
	Animation(float dur);
	void addFrame(const std::shared_ptr<Texture>& fr);
	void addFrame(const std::string& fp);

	void Play(float deltaTime);
	void Render(const vec2& position, const vec2& size);

private:
	std::vector<std::shared_ptr<Texture>> m_Frames;

	int m_FrameIndex = 0;

	float m_FrameDuration;
	float m_LastFrame;

};