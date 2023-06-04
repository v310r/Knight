#include "Application.h"
#include <iostream>


Application::Application() : 
	m_Window("Knight", sf::Vector2u(800, 600)), m_StateManager(&m_SharedContext), m_EntityManager(&m_SharedContext, 10)
{
	m_SharedContext.SetWindow(&m_Window);
	m_SharedContext.SetEventManager(m_Window.GetEventManager());
	m_SharedContext.SetEntityManager(&m_EntityManager);
	m_SharedContext.SetTextureManager(&m_TextureManager);
	m_StateManager.SwitchTo(StateType::Game);
}

Application::~Application()
{

}

void Application::HandleInput()
{

}

void Application::Update()
{
	const sf::Time d = m_clock.restart();
	m_Window.Update();
	//std::cout << "deltaTime: " << d.asSeconds() << "\n";
	m_StateManager.Update(d.asSeconds());
}

void Application::Render()
{
	m_Window.BeginDraw();
	m_StateManager.Draw();
	m_Window.EndDraw();
}

Window& Application::GetWindow()
{
	return m_Window;
}

void Application::PostUpdate()
{
	m_StateManager.ProcessRequests();
}
