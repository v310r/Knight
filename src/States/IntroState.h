#pragma once

#include "BaseState.h"


class IntroState : public BaseState
{
public:

	IntroState(StateManager* stateManager) : BaseState(stateManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const sf::Time& deltaTime) override;
	virtual void Draw() override;

	void Continue(EventDetails* details);

private:

	sf::Texture m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;
	sf::Font m_font;
	float m_timePassed;
};

