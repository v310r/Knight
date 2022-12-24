#pragma once


#include <SFML/Graphics.hpp>
#include <string>
#include "../Events/EventManager.h"
#include <memory>

class Window
{
public:
	Window();

	Window(const std::string& title, const sf::Vector2u& size);

	~Window();

	// Clear the window
	void BeginDraw();

	// Display changes
	void EndDraw();

	void Update();

	bool IsDone() const;

	bool IsFullscreen() const;

	sf::Vector2u GetWindowSize();

	void Draw(sf::Drawable& drawable);

	sf::RenderWindow* GetRenderWindow()
	{
		return &m_window;
	}

	bool IsFocused();

	EventManager* GetEventManager();

	void ToggleFullscreen(EventDetails* details);

	void Close(EventDetails* details = nullptr);

private:

	void Setup(const std::string& title, const sf::Vector2u size);

	void Destroy();

	void Create();


	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;
	EventManager m_eventManager;
	bool m_isFocused;
};