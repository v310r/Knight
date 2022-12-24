#pragma once

#include "../Window/Window.h"
#include <iostream>
#include "../States/StateManager.h"


class Game
{
public:
	Game();

	~Game();

	void HandleInput();

	void Update();

	void Render();

	Window& GetWindow();

	void PostUpdate();

private:

	Window m_window;
	sf::Clock m_clock;
	SharedContext m_context;
	StateManager m_stateManager;
};
