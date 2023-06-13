#include "PausedState.h"

#include "StateManager.h"
#include "Window/Window.h"


void PausedState::OnCreate()
{
	m_Transparent = true;
	SetTransparent(true); // Set our transparency flag.
	m_font.loadFromFile("assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	const sf::Vector2u windowSize = GetStateManager()->GetContext()->GetWindow()->GetRenderWindow()->getSize();
	const sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));

	std::shared_ptr<EventManager> eManager = GetStateManager()->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Paused, "Key_P",&PausedState::Unpause, this);
}

void PausedState::OnDestroy()
{
	std::shared_ptr<EventManager> eManager = GetStateManager()->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Paused, "Key_P");
}

void PausedState::Activate()
{
}

void PausedState::Deactivate()
{
}

void PausedState::Update(const float deltaTime)
{
}

void PausedState::Draw()
{
	sf::RenderWindow* window = GetStateManager()->GetContext()->GetWindow()->GetRenderWindow();
	window->draw(m_rect);
	window->draw(m_text);
}

void PausedState::Unpause(EventDetails* details)
{
	GetStateManager()->SwitchTo(StateType::Game);
}
