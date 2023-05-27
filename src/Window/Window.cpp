#include "Window.h"


Window::Window()
{
	Setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{
	Setup(title, size);
}

Window::~Window()
{
	Destroy();
}

void Window::BeginDraw()
{
	m_Window.clear(sf::Color::Black);
}

void Window::EndDraw()
{
	m_Window.display();
}

void Window::Update()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus)
		{
			m_IsFocused = false;
			m_EventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_IsFocused = true;
			m_EventManager.SetFocus(true);
		}
		m_EventManager.HandleEvent(event);
	}
	m_EventManager.Update();
}

bool Window::IsDone() const
{
	return m_IsDone;
}

bool Window::IsFullscreen() const
{
	return m_IsFullscreen;
}

sf::Vector2u Window::GetWindowSize()
{
	return m_WindowSize;
}

void Window::Draw(sf::Drawable& drawable)
{
	m_Window.draw(drawable);
}

bool Window::IsFocused()
{
	return false;
}

EventManager* Window::GetEventManager()
{
	return &m_EventManager;
}

void Window::ToggleFullscreen(EventDetails* details)
{
	m_IsFullscreen = !m_IsFullscreen;
	Destroy();
	Create();
}

void Window::Close(EventDetails* details)
{
	m_IsDone = true;
}

sf::FloatRect Window::GetViewSpace()
{
	const sf::Vector2f viewCenter = m_Window.getView().getCenter();
	const sf::Vector2f viewSize = m_Window.getView().getSize();
	const sf::Vector2f viewSizeHalf = viewSize / 2.0f;
	const sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	return viewSpace;
}

void Window::Setup(const std::string& title, const sf::Vector2u size)
{
	m_WindowTitle = title;
	m_WindowSize = size;
	m_IsFullscreen = false;
	m_IsDone = false;
	Create();

	m_IsFocused = true;
	m_EventManager.AddCallback(StateType::Global, "FullscreenToggle", &Window::ToggleFullscreen, this);
	m_EventManager.AddCallback(StateType::Global, "WindowClose", &Window::Close, this);
}

void Window::Destroy()
{
	m_Window.close();
}

void Window::Create()
{
	const auto style = (m_IsFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_Window.create({ m_WindowSize.x, m_WindowSize.y, 32 }, m_WindowTitle, style);
}
