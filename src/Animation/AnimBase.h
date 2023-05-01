#pragma once

#include <sstream>

class SpriteSheet;

using Frame = unsigned int;


class AnimBase
{
public:
	AnimBase();
	virtual ~AnimBase() = 0;

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(float deltaTime);

	friend std::stringstream& operator>>(std::stringstream& stream, AnimBase& a)
	{
		a.ReadIn(stream);
		return stream;
	}

	bool IsInAction();

	void SetSpriteSheet(SpriteSheet* const sheet);
	void SetCurrentFrame(Frame frame);
	void SetLooping(bool value);
	void SetName(const std::string& name);

protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

	Frame m_CurrentFrame = 0;
	Frame m_StartFrame = 0;
	Frame m_EndFrame = 0;
	Frame m_FrameRow = 0;


	// Action defines a range of frames during which a behavior which
	// is specific to that animation can be performed.If it's set to negative 
	// one, this behavior can be performed throughout the entire animation

	int m_ActionStartFrame = -1; // Frame when a specific "action" begins
	int m_ActionEndFrame = -1;  // Frame when a specific "action" ends

	float m_FrameTime = 0.0f;
	float m_ElapsedTime = 0.0f;

	bool m_bLoop = false;
	bool m_bPlaying = false;

	std::string m_Name;
	SpriteSheet* m_SpriteSheet;

private:
	friend class SpriteSheet;
};

