#pragma once


#include <SFML/Graphics.hpp>
#include <string>
#include "Events/EventManager.h"
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
		return &m_Window;
	}

	bool IsFocused();

	std::shared_ptr<EventManager>& GetEventManager();

	void ToggleFullscreen(EventDetails* details);

	void Close(EventDetails* details = nullptr);

	sf::FloatRect GetViewSpace();

private:

	void Setup(const std::string& title, const sf::Vector2u size);

	void Destroy();

	void Create();


	sf::RenderWindow m_Window;
	sf::Vector2u m_WindowSize;
	std::string m_WindowTitle;

	bool m_IsDone;
	bool m_IsFullscreen;
	bool m_IsFocused;

	std::shared_ptr<EventManager> m_EventManager = std::make_shared<EventManager>();

};