#pragma once

#include "Window/Window.h"
#include <iostream>
#include "States/StateManager.h"
#include "Entities/EntityManager.h"
#include "ResourceManagement/TextureManager.h"
#include <memory>


class Application
{
public:
	Application();

	~Application();

	void HandleInput();

	void Update();

	void Render();

	std::shared_ptr<Window>& GetWindow();

	void PostUpdate();

private:


	sf::Clock m_clock;

	SharedContext m_SharedContext;

	std::shared_ptr<TextureManager> m_TextureManager;
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<StateManager> m_StateManager;
};
