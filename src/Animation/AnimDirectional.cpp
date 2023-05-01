#include "AnimDirectional.h"
#include "SpriteSheet.h"


void AnimDirectional::FrameStep()
{
	if (m_StartFrame < m_EndFrame)
	{
		++m_CurrentFrame;
	}
	else
	{
		--m_CurrentFrame;
	}

	if ((m_StartFrame < m_EndFrame && m_CurrentFrame > m_EndFrame)
		|| (m_StartFrame > m_EndFrame && m_CurrentFrame < m_EndFrame))
	{
		if (m_bLoop)
		{
			m_CurrentFrame = m_StartFrame;
			return;
		}

		m_CurrentFrame = m_EndFrame;
		Pause();
	}

}

void AnimDirectional::CropSprite()
{
	const int LeftX = m_SpriteSheet->GetSpriteSize().x * m_CurrentFrame;
	const int LeftY = m_SpriteSheet->GetSpriteSize().y * (m_FrameRow + static_cast<unsigned int>(m_SpriteSheet->GetDirection()));
	const int SizeX = m_SpriteSheet->GetSpriteSize().x;
	const int SizeY = m_SpriteSheet->GetSpriteSize().y;

	const sf::IntRect rect(LeftX, LeftY, SizeX, SizeY);
	m_SpriteSheet->CropSprite(rect);
}

void AnimDirectional::ReadIn(std::stringstream& stream)
{
	stream >> m_StartFrame >> m_EndFrame >> m_FrameRow >> m_FrameTime >> m_ActionStartFrame
		>> m_ActionEndFrame;
}
