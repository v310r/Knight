#include "IntroState.h"

#include "StateManager.h"
#include "Window/Window.h"


void IntroState::OnCreate()
{
	m_timePassed = 0.0f;
	const sf::Vector2u windowSize = m_StateManager->GetContext()->GetWindow()->GetRenderWindow()->getSize();

	m_introTexture.loadFromFile("assets/textures/intro.png");
	m_introSprite.setTexture(m_introTexture);

	m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f, m_introTexture.getSize().y / 2.0f);
	m_introSprite.setPosition(windowSize.x / 2.0f, 0.0f);

	m_font.loadFromFile("assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString({ "Press SPACE to continue" });
	m_text.setCharacterSize(15);
	const sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Intro, "IntroContinue", &IntroState::Continue, this);
}

void IntroState::OnDestroy()
{
	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Intro, "IntroContinue");

}

void IntroState::Activate()
{
}

void IntroState::Deactivate()
{
}

void IntroState::Update(const sf::Time& deltaTime)
{
	if (m_timePassed < 5.0f)
	{
		m_timePassed += deltaTime.asSeconds() * 50;
		m_introSprite.setPosition(m_introSprite.getPosition().x,
			m_introSprite.getPosition().y + (48 * 50 * deltaTime.asSeconds()));
	}
}

void IntroState::Draw()
{
	sf::RenderWindow* window = m_StateManager->GetContext()->GetWindow()->GetRenderWindow();
	window->draw(m_introSprite);
	if (m_timePassed >= 5.0f)
	{
		window->draw(m_text);
	}
}

void IntroState::Continue(EventDetails* details)
{
	if (m_timePassed >= 5.0f)
	{
		m_StateManager->SwitchTo(StateType::MainMenu);
		m_StateManager->Remove(StateType::Intro);
	}
}
