#include "GameState.h"
#include "StateManager.h"
#include "../Window/Window.h"


void GameState::OnCreate()
{
	m_texture.loadFromFile("assets/textures/Reaper.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(0, 0);
	m_increment = sf::Vector2f(400.0f, 400.0f);

	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Game, "Key_Escape", &GameState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);
}

void GameState::OnDestroy()
{
	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Game, "Key_Escape");
	eManager->RemoveCallback(StateType::Game, "Key_P");
}

void GameState::Activate()
{
}

void GameState::Deactivate()
{
}

void GameState::Update(const sf::Time& deltaTime)
{
	const sf::Vector2u windowSize = m_StateManager->GetContext()->GetWindow()->GetWindowSize();
	const sf::Vector2u textureSize = m_texture.getSize();
	if ((m_sprite.getPosition().x > windowSize.x -
		textureSize.x && m_increment.x > 0) ||
		(m_sprite.getPosition().x < 0 && m_increment.x < 0))
	{
		m_increment.x = -m_increment.x;
	}
	if ((m_sprite.getPosition().y > windowSize.y -
		textureSize.y && m_increment.y > 0) ||
		(m_sprite.getPosition().y < 0 && m_increment.y < 0))
	{
		m_increment.y = -m_increment.y;
	}
	m_sprite.setPosition(m_sprite.getPosition().x +
		(m_increment.x * deltaTime.asSeconds()),
		m_sprite.getPosition().y +
		(m_increment.y * deltaTime.asSeconds()));
}

void GameState::Draw()
{
	m_StateManager->GetContext()->GetWindow()->GetRenderWindow()->draw(m_sprite);
}

void GameState::GoToMainMenu(EventDetails* details)
{
	m_StateManager->SwitchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details)
{
	m_StateManager->SwitchTo(StateType::Paused);
}
