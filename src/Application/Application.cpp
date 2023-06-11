#include "Application.h"
#include <iostream>

#ifdef _DEBUG
constexpr float FIXED_DELTA_TIME = 0.0005f;
#endif

Application::Application() : 
	m_Window("Knight", sf::Vector2u(800, 600))
{
	m_StateManager = std::make_unique<StateManager>(&m_SharedContext);
	m_EntityManager = std::make_unique<EntityManager>(&m_SharedContext, 10);
	m_TextureManager = std::make_unique<TextureManager>();


	m_SharedContext.SetWindow(&m_Window);
	m_SharedContext.SetEventManager(m_Window.GetEventManager());
	m_SharedContext.SetEntityManager(m_EntityManager.get());
	m_SharedContext.SetTextureManager(m_TextureManager.get());
	m_StateManager->SwitchTo(StateType::Game);
}

Application::~Application()
{
	m_StateManager.reset(nullptr);
	m_EntityManager.reset(nullptr);
	m_TextureManager.reset(nullptr);
}

void Application::HandleInput()
{

}

void Application::Update()
{
#ifndef _DEBUG
	const sf::Time d = m_clock.restart();
#endif

	m_Window.Update();

#ifndef _DEBUG
	//std::cout << "deltaTime: " << d.asSeconds() << "\n";
	const float deltaTime = d.asSeconds();
#else
	const float deltaTime = FIXED_DELTA_TIME;
#endif

	m_StateManager->Update(deltaTime);
}

void Application::Render()
{
	m_Window.BeginDraw();
	m_StateManager->Draw();
	m_Window.EndDraw();
}

Window& Application::GetWindow()
{
	return m_Window;
}

void Application::PostUpdate()
{
	m_StateManager->ProcessRequests();
}
