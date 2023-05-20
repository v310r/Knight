#include "Application.h"

Application::Application() : m_Window("Knight", sf::Vector2u(800, 600)), m_StateManager(&m_SharedContext)
{
	m_SharedContext.SetWindow(&m_Window);
	m_SharedContext.SetEventManager(m_Window.GetEventManager());
	m_StateManager.SwitchTo(StateType::Intro);
}

Application::~Application()
{
}

void Application::HandleInput()
{

}

void Application::Update()
{
	m_Window.Update();
	const sf::Time d = m_clock.getElapsedTime();
	m_StateManager.Update(d);
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
	m_clock.restart();
}
