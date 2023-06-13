#include "GameState.h"

#include <iostream>
#include "StateManager.h"
#include "Window/Window.h"
#include "World/Map.h"
#include "Entities/EntityManager.h"
#include "Entities/EntityBase.h"

void PrintMemoryUsage();

GameState::GameState(const std::shared_ptr<StateManager>& stateManager) : BaseState(stateManager)
{

}

void GameState::OnCreate()
{
	const sf::Vector2u size = GetStateManager()->GetContext()->GetWindow()->GetWindowSize();

	m_View.setSize(static_cast<float>(size.x), static_cast<float>(size.y));
	m_View.setCenter(size.x / 2.0f, size.y / 2.0f);
	m_View.zoom(0.6f);
	GetStateManager()->GetContext()->GetWindow()->GetRenderWindow()->setView(m_View);

	m_Map = std::make_shared<Map>(GetStateManager()->GetContext(), this);
	m_Map->AssociateWithCurrentContext();
	m_Map->LoadMap("cfg/Maps/Map1.map");

	std::shared_ptr<EventManager> eManager = GetStateManager()->GetContext()->GetEventManager();
	eManager->AddCallback(StateType::Game, "Key_Escape", &GameState::GoToMainMenu, this);
	eManager->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);
}

void GameState::OnDestroy()
{
	std::shared_ptr<EventManager> eManager = GetStateManager()->GetContext()->GetEventManager();
	eManager->RemoveCallback(StateType::Game, "Key_Escape");
	eManager->RemoveCallback(StateType::Game, "Key_P");

	m_Map.reset();
}

void GameState::Activate()
{
}

void GameState::Deactivate()
{
}

void GameState::Update(const float deltaTime)
{
	SharedContext* context = GetStateManager()->GetContext();
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
	GetStateManager()->GetContext()->GetEntityManager()->Update(deltaTime);
}

void GameState::Draw()
{
	m_Map->Draw();
	GetStateManager()->GetContext()->GetEntityManager()->Draw();
}

void GameState::GoToMainMenu(EventDetails* details)
{
	GetStateManager()->SwitchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details)
{
	GetStateManager()->SwitchTo(StateType::Paused);
}
