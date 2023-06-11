#include "GameState.h"

#include <iostream>
#include "StateManager.h"
#include "Window/Window.h"
#include "World/Map.h"
#include "Entities/EntityManager.h"
#include "Entities/EntityBase.h"


GameState::GameState(StateManager* stateManager) : BaseState(stateManager)
{

}

void GameState::OnCreate()
{
	const sf::Vector2u size = m_StateManager->GetContext()->GetWindow()->GetWindowSize();

	m_View.setSize(size.x, size.y);
	m_View.setCenter(size.x / 2.0f, size.y / 2.0f);
	m_View.zoom(0.6f);
	m_StateManager->GetContext()->GetWindow()->GetRenderWindow()->setView(m_View);

	m_Map = new Map(m_StateManager->GetContext(), this);
	m_Map->LoadMap("cfg/Maps/Map1.map");

	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Game, "Key_Escape", &GameState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);
}

void GameState::OnDestroy()
{
	EventManager* const eManager = m_StateManager->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Game, "Key_Escape");
	eManager->RemoveCallback(StateType::Game, "Key_P");

	delete m_Map;
	m_Map = nullptr;
}

void GameState::Activate()
{
}

void GameState::Deactivate()
{
}

void GameState::Update(const float deltaTime)
{
	SharedContext* context = m_StateManager->GetContext();
	EntityBase* player = context->GetEntityManager()->Find("Player");
	if (!player)
	{
		std::cout << "Respawning player..." << ", src: " << __FILE__ <<  std::endl;
		context->GetEntityManager()->Add(EntityType::Player, "Player");
		player = context->GetEntityManager()->Find("Player");
		player->SetPosition(m_Map->GetPlayerStart());
	}
	else
	{
		m_View.setCenter(player->GetPosition());
		context->GetWindow()->GetRenderWindow()->setView(m_View);
	}

	const sf::FloatRect viewSpace = context->GetWindow()->GetViewSpace();
	if (viewSpace.left <= 0.0f)
	{
		m_View.setCenter(viewSpace.width / 2.0f, m_View.getCenter().y);
		context->GetWindow()->GetRenderWindow()->setView(m_View);
	}
	else if (viewSpace.left + viewSpace.width > m_Map->GetMapSize().x * TileSheet::TileSize)
	{
		m_View.setCenter((m_Map->GetMapSize().x * TileSheet::TileSize) - (viewSpace.width / 2.0f), m_View.getCenter().y);
		context->GetWindow()->GetRenderWindow()->setView(m_View);
	}

	m_Map->Update(deltaTime);
	m_StateManager->GetContext()->GetEntityManager()->Update(deltaTime);
}

void GameState::Draw()
{
	m_Map->Draw();
	m_StateManager->GetContext()->GetEntityManager()->Draw();
}

void GameState::GoToMainMenu(EventDetails* details)
{
	m_StateManager->SwitchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details)
{
	m_StateManager->SwitchTo(StateType::Paused);
}
