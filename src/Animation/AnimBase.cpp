#include "AnimBase.h"
#include "SpriteSheet.h"


AnimBase::AnimBase()
{
}

AnimBase::~AnimBase()
{
}

void AnimBase::Play()
{	
	m_bPlaying = true;
}

void AnimBase::Pause()
{
	m_bPlaying = false;
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

void AnimBase::SetSpriteSheet(SpriteSheet* const sheet)
{
	m_SpriteSheet = sheet;
}

void AnimBase::SetCurrentFrame(Frame frame)
{
	if ((frame >= m_StartFrame && frame <= m_EndFrame) ||
		frame >= m_EndFrame && frame <= m_StartFrame)
	{
		m_CurrentFrame = frame;
	}
}

void AnimBase::SetLooping(bool value)
{
	m_bLoop = value;
}

void AnimBase::SetName(const std::string& name)
{
	m_Name = name;
}
