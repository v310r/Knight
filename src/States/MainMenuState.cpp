#include "MainMenuState.h"

#include "StateManager.h"
#include "Window/Window.h"


void MainMenuState::OnCreate()
{
	m_font.loadFromFile("assets/fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("MAIN MENU:"));
	m_text.setCharacterSize(18);

	const sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(400, 100);
	m_buttonSize = sf::Vector2f(300.0f, 32.0f);
	m_buttonPos = sf::Vector2f(400, 200);
	m_buttonPadding = 4; // 4px.

	std::string optionNames[3];
	optionNames[0] = "PLAY";
	optionNames[1] = "CREDITS";
	optionNames[2] = "EXIT";
	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y +
			(i * (m_buttonSize.y + m_buttonPadding)));
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color::Red);
		m_rects[i].setOrigin(m_buttonSize.x / 2.0f,
			m_buttonSize.y / 2.0f);
		m_rects[i].setPosition(buttonPosition);
		m_labels[i].setFont(m_font);
		m_labels[i].setString(sf::String(optionNames[i]));
		m_labels[i].setCharacterSize(12);
		sf::FloatRect rect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
		m_labels[i].setPosition(buttonPosition);
	}

	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::MainMenu, "MouseClick", &MainMenuState::MouseClick, this);
}

void MainMenuState::OnDestroy()
{
	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::MainMenu, "MouseClick");
}

void MainMenuState::Activate()
{
	if (m_StateManager->HasState(StateType::Game) && m_labels[0].getString() == "PLAY")
	{
		m_labels[0].setString(sf::String("RESUME"));
		const sf::FloatRect rect = m_labels[0].getLocalBounds();
		m_labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	}
}

void MainMenuState::Deactivate()
{
}

void MainMenuState::Update(const sf::Time& deltaTime)
{
}

void MainMenuState::Draw()
{
	sf::RenderWindow* window = m_StateManager->GetContext()->GetWindow()->GetRenderWindow();
	window->draw(m_text);
	for (size_t i = 0; i < 3; ++i)
	{
		window->draw(m_rects[i]);
		window->draw(m_labels[i]);
	}
}

void MainMenuState::MouseClick(EventDetails* details)
{
	const sf::Vector2i mousePos = details->GetMousePos();
	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;
	for (int i = 0; i < 3; ++i) 
	{
		if (mousePos.x >= m_rects[i].getPosition().x - halfX &&
			mousePos.x <= m_rects[i].getPosition().x + halfX &&
			mousePos.y >= m_rects[i].getPosition().y - halfY &&
			mousePos.y <= m_rects[i].getPosition().y + halfY)
		{
			if (i == 0)
			{
				m_StateManager->SwitchTo(StateType::Game);
			}
			else if (i == 1)
			{
				// Credits state.
			}
			else if (i == 2)
			{
				m_StateManager->GetContext()->GetWindow()->Close();
			}
		}
	}
}
