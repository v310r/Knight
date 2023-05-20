#pragma once

#include "../Window/Window.h"
#include <iostream>
#include "../States/StateManager.h"


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

	Window m_Window;
	sf::Clock m_clock;
	SharedContext m_SharedContext;
	StateManager m_StateManager;
};
