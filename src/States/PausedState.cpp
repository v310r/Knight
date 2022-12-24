#include "PausedState.h"
#include "StateManager.h"

void PausedState::OnCreate()
{
	m_transparent = true;
	SetTransparent(true); // Set our transparency flag.
	m_font.loadFromFile("assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	const sf::Vector2u windowSize = m_stateManager->
		GetContext()->GetWindow()->GetRenderWindow()->getSize();
	const sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));

	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Paused, "Key_P",&PausedState::Unpause, this);
}

void PausedState::OnDestroy()
{
	EventManager* const eManager = m_stateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Paused, "Key_P");
}

void PausedState::Activate()
{
}

void PausedState::Deactivate()
{
}

void PausedState::Update(const sf::Time& deltaTime)
{
}

void PausedState::Draw()
{
	sf::RenderWindow* window = m_stateManager->GetContext()->GetWindow()->GetRenderWindow();
	window->draw(m_rect);
	window->draw(m_text);
}

void PausedState::Unpause(EventDetails* details)
{
	m_stateManager->SwitchTo(StateType::Game);
}
