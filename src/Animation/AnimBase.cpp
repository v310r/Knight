#include "AnimBase.h"
#include "SpriteSheet.h"


AnimBase::AnimBase()
{
}

AnimBase::~AnimBase()
{
}

void AnimBase::Stop()
{
	m_bPlaying = false;
	Reset();
}

void AnimBase::Reset()
{
	m_CurrentFrame = m_StartFrame;
	m_ElapsedTime = 0.0f;
	CropSprite();
}

void AnimBase::Update(float deltaTime)
{
	if (!m_bPlaying)
	{
		return;
	}

	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime < m_FrameTime)
	{
		return;
	}

	FrameStep();
	CropSprite();
	m_ElapsedTime = 0.0f;
}

bool AnimBase::IsInAction()
{
	if (m_ActionStartFrame == -1 || m_ActionEndFrame == -1)
	{
		return true;
	}

	return (m_CurrentFrame >= m_ActionStartFrame && m_CurrentFrame <= m_ActionEndFrame);
}

void AnimBase::SetCurrentFrame(Frame frame)
{
	if ((frame >= m_StartFrame && frame <= m_EndFrame) ||
		frame >= m_EndFrame && frame <= m_StartFrame)
	{
		m_CurrentFrame = frame;
	}
}
