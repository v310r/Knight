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

	Window& GetWindow();

	void PostUpdate();

private:

	std::unique_ptr<TextureManager> m_TextureManager;

	Window m_Window;
	sf::Clock m_clock;

	SharedContext m_SharedContext;

	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<StateManager> m_StateManager;
};
