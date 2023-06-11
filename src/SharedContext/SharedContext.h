#pragma once


class Window;
class EventManager;
class TextureManager;
class Map;
class EntityManager;

#include <iostream>

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

	TextureManager* GetTextureManager() const
	{
		return m_TextureManager;
	}

	void SetTextureManager(TextureManager* textureManager)
	{
		m_TextureManager = textureManager;
	}

	Map* GetMap() const
	{
		return m_Map;
	}

	void SetMap(Map* map)
	{
		m_Map = map;
	}

	EntityManager* GetEntityManager() const
	{
		return m_EntityManager;
	}

	void SetEntityManager(EntityManager* entityManager)
	{
		m_EntityManager = entityManager;
	}

private:

	Window* m_Window = nullptr;
	EventManager* m_EventManager = nullptr;
	TextureManager* m_TextureManager = nullptr;
	Map* m_Map = nullptr;
	EntityManager* m_EntityManager = nullptr;
};
