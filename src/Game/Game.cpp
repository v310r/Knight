#include "Game.h"

Game::Game() : m_Window("Knight", sf::Vector2u(800, 600)), m_StateManager(&m_SharedContext)
{
	m_SharedContext.SetWindow(&m_Window);
	m_SharedContext.SetEventManager(m_Window.GetEventManager());
	m_StateManager.SwitchTo(StateType::Intro);
}

Game::~Game()
{
}

void Game::HandleInput()
{

}

void Game::Update()
{
	m_Window.Update();
	const sf::Time d = m_clock.getElapsedTime();
	m_StateManager.Update(d);
}

void Game::Render()
{
	m_Window.BeginDraw();
	m_StateManager.Draw();
	m_Window.EndDraw();
}

Window& Game::GetWindow()
{
	return m_Window;
}

void Game::PostUpdate()
{
	m_StateManager.ProcessRequests();
	m_clock.restart();
}
