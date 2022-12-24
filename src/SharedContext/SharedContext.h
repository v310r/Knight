#pragma once

#include "../Window/Window.h"
#include "../Events/EventManager.h"

class SharedContext
{
public:

	SharedContext() : m_window(nullptr), m_eventManager(nullptr) {}

	Window* GetWindow() const
	{
		return m_window;
	}

	void SetWindow(Window* window)
	{
		m_window = window;
	}

	EventManager* GetEventManager() const
	{
		return m_eventManager;
	}

	void SetEventManager(EventManager* eventManager)
	{
		m_eventManager = eventManager;
	}

private:

	Window* m_window;
	EventManager* m_eventManager;
};
