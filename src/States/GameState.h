#pragma once

#include "BaseState.h"


class GameState : public BaseState
{
public:

	GameState(StateManager* stateManager) : BaseState(stateManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const sf::Time& deltaTime) override;
	virtual void Draw() override;

	void GoToMainMenu(EventDetails* details);
	void Pause(EventDetails* details);

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_increment;
};

