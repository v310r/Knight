#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include "../States/StateType.h"

enum class EventType
{
	KeyPressed = sf::Event::KeyPressed,
	KeyReleased = sf::Event::KeyReleased,
	MouseButtonPressed = sf::Event::MouseButtonPressed,
	MouseButtonReleased = sf::Event::MouseButtonReleased,
	MouseWheelMoved = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1,
	Mouse, 
	Joystick
};

struct EventInfo
{
	EventInfo()
	{
		m_code = 0;
	}

	EventInfo(const int eventCode)
	{
		m_code = eventCode;
	}

	int GetCode() const
	{
		return m_code;
	}

	void SetCode(const int value)
	{
		m_code = value;
	}

private:

	union
	{
		int m_code; // button code
	};
};

struct EventDetails
{
	EventDetails(const std::string& bindName) : m_name(bindName)
	{
		Clear();
	}

	void Clear()
	{
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}

	std::string GetName() const
	{
		return m_name;
	}

	void SetSize(const int width, const int height)
	{
		m_size = {width, height};
	}

	sf::Vector2i GetSize() const
	{
		return m_size;
	}

	void SetMousePos(const sf::Uint32 textEntered)
	{
		m_textEntered = textEntered;
	}

	sf::Uint32 GetTextEntered() const
	{
		return m_textEntered;
	}

	void SetTextEntered(const sf::Uint32 textEntered)
	{
		m_textEntered = textEntered;
	}

	void SetMousePos(const sf::Vector2i mouse)
	{
		m_mouse = mouse;
	}

	sf::Vector2i GetMousePos() const
	{
		return m_mouse;
	}

	void SetMouseWheelDelta(const int mouseWheelDelta)
	{
		m_mouseWheelDelta = mouseWheelDelta;
	}

	int GetMouseWheelDelta() const
	{
		return m_mouseWheelDelta;
	}

	void SetKeyCode(const int keyCode)
	{
		m_keyCode = keyCode;
	}

	int GetKeyCode() const
	{
		return m_keyCode;
	}

private:

	std::string m_name;
	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;
	int m_mouseWheelDelta;
	int m_keyCode;
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding
{
	Binding(const std::string& name) : m_name(name), m_details(name), m_count(0)
	{

	}

	void BindEvent(EventType type, EventInfo info = EventInfo())
	{
		m_events.emplace_back(type, info);
	}

	std::string GetName() const
	{
		return m_name;
	}

	Events& GetEvents()
	{
		return m_events;
	}

	const Events& GetEvents() const
	{
		return m_events;
	}

	EventDetails& GetDetails()
	{
		return m_details;
	}

	const EventDetails& GetDetails() const
	{
		return m_details;
	}

	void SetCount(const int count)
	{
		m_count = count;
	}

	int GetCount()
	{
		return m_count;
	}

private:

	Events m_events;
	std::string m_name;
	EventDetails m_details;
	int m_count; // Count of events that are "happening"
};

using Bindings = std::unordered_map<std::string, Binding*>;
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager
{
public:

	EventManager();
	~EventManager();

	bool AddBinding(Binding* binding);

	bool RemoveBinding(const std::string& name);

	void SetFocus(const bool& focus);

	template<typename T>
	bool AddCallback(const StateType state, const std::string& name, void(T::* func)(EventDetails*), T* instance)
	{
		auto i = m_callbacks.emplace(state, CallbackContainer()).first;
		const auto temp = std::bind(func, instance, std::placeholders::_1);
		return i->second.emplace(name, temp).second;
	}

	bool RemoveCallback(const StateType state, const std::string& name);

	void HandleEvent(const sf::Event& incomingEvent);

	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* window = nullptr)
	{
		return window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition();
	}

	void SetCurrentState(const StateType type)
	{
		m_currentState = type;
	}

private:

	void LoadBindings();

	Bindings m_bindings;
	Callbacks m_callbacks;
	StateType m_currentState;
	bool m_hasFocus;
};
