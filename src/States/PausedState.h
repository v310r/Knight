#pragma once

#include "BaseState.h"


class PausedState : public BaseState
{
public:

	PausedState(StateManager* stateManager) : BaseState(stateManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const float deltaTime) override;
	virtual void Draw() override;

	void Unpause(EventDetails* details);

private:

	sf::Text m_text;
	sf::Font m_font;
	sf::RectangleShape m_rect;
};

