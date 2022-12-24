#include "Game.h"

Game::Game() : m_window("Knight", sf::Vector2u(800, 600)), m_stateManager(&m_context)
{
	m_context.SetWindow(&m_window);
	m_context.SetEventManager(m_window.GetEventManager());
	m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game()
{
}

void Game::HandleInput()
{

}

void Game::Update()
{
	m_window.Update();
	const sf::Time d = m_clock.getElapsedTime();
	m_stateManager.Update(d);
}

void Game::Render()
{
	m_window.BeginDraw();
	m_stateManager.Draw();
	m_window.EndDraw();
}

Window& Game::GetWindow()
{
	return m_window;
}

void Game::PostUpdate()
{
	m_stateManager.ProcessRequests();
	m_clock.restart();
}
