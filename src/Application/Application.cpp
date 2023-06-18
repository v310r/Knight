#include "Application.h"
#include <iostream>
#include "Utilities/Profiling.h"

#ifdef _DEBUG
constexpr float FIXED_DELTA_TIME = 0.0005f;
#endif


Application::Application()
{
	m_Window = std::make_shared<Window>("Knight", sf::Vector2u(800, 600));
	m_StateManager = std::make_shared<StateManager>(&m_SharedContext);
	m_EntityManager = std::make_shared<EntityManager>(&m_SharedContext, 10);
	m_TextureManager = std::make_shared<TextureManager>();


	m_SharedContext.SetWindow(m_Window);
	m_SharedContext.SetEventManager(m_Window->GetEventManager());
	m_SharedContext.SetEntityManager(m_EntityManager);
	m_SharedContext.SetTextureManager(m_TextureManager);
	m_StateManager->SwitchTo(StateType::Game);
}

Application::~Application()
{
	m_StateManager->PurgeStates();
	m_EntityManager.reset();
	m_Window.reset();
	m_TextureManager->PurgeResources();
}

void Application::HandleInput()
{

}

void Application::Update()
{
#ifndef _DEBUG
	const sf::Time d = m_clock.restart();
#endif

	m_Window->Update();

#ifdef _DEBUG
	const float deltaTime = FIXED_DELTA_TIME;
#else
	const float deltaTime = d.asSeconds();
#endif

	m_StateManager->Update(deltaTime);
}

void Application::Render()
{
	m_Window->BeginDraw();
	m_StateManager->Draw();
	m_Window->EndDraw();
}

std::shared_ptr<Window>& Application::GetWindow()
{
	return m_Window;
}

void Application::PostUpdate()
{
	m_StateManager->ProcessRequests();
}
