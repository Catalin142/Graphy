#include "Core/GrPch.h"
#include "Animation.h"
#include "Renderer/Renderer.h"

Animation::Animation(float dur) : m_FrameDuration(dur), m_LastFrame(m_FrameDuration)
{ }

void Animation::addFrame(const std::shared_ptr<Texture>& fr)
{
	m_Frames.push_back(fr);
}

void Animation::addFrame(const std::string& fp)
{
	m_Frames.push_back(std::make_shared<Texture>(fp));
}

void Animation::Play(float deltaTime)
{
	if (m_LastFrame > m_FrameDuration)
	{
		m_FrameIndex++;
		if (m_FrameIndex > m_Frames.size() - 1)
			m_FrameIndex = 0;
		m_LastFrame = 0;
	}

	m_LastFrame += deltaTime;
}

void Animation::Render(const vec2& position, const vec2& size)
{
	if (m_Frames.size() > 0)
		Renderer::renderTexture(m_Frames[m_FrameIndex], position, size);
}
