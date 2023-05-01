#pragma once

#include "../Window/Window.h"
#include "../Events/EventManager.h"

class SharedContext
{
public:

	SharedContext() : m_Window(nullptr), m_EventManager(nullptr) {}

	Window* GetWindow() const
	{
		return m_Window;
	}

	void SetWindow(Window* window)
	{
		m_Window = window;
	}

	EventManager* GetEventManager() const
	{
		return m_EventManager;
	}

	void SetEventManager(EventManager* eventManager)
	{
		m_EventManager = eventManager;
	}

private:

	Window* m_Window;
	EventManager* m_EventManager;
};
