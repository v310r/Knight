#pragma once
#include "BaseState.h"


class MainMenuState : public BaseState
{
public:

	MainMenuState(StateManager* stateManager) : BaseState(stateManager) {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const sf::Time& deltaTime) override;
	virtual void Draw() override;

	void MouseClick(EventDetails* details);

private:

	sf::Text m_text;
	sf::Font m_font;
	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];
};

