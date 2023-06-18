#pragma once

#include <sstream>

class SpriteSheet;

using Frame = int;


class AnimBase
{
	friend class SpriteSheet;

public:
	AnimBase();
	virtual ~AnimBase() = 0;

	void Play() { m_bPlaying = true; }
	void Pause() { m_bPlaying = false; }
	void Stop();
	void Reset();

	virtual void Update(float deltaTime);

	friend std::stringstream& operator>>(std::stringstream& stream, AnimBase& a)
	{
		a.ReadIn(stream);
		return stream;
	}

	bool IsInAction();

	void SetSpriteSheet(SpriteSheet* const sheet) { m_SpriteSheet = sheet; }

	void SetCurrentFrame(Frame frame);

	void SetLooping(bool value) { m_bLoop = value; }
	bool IsLooping() const { return m_bLoop; }

	bool IsPlaying() const { return m_bPlaying; }

	void SetName(const std::string& name) { m_Name = name; }
	const std::string& GetName() const { return m_Name; }

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
};

