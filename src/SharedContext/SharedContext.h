#pragma once


class Window;
class EventManager;
class TextureManager;
class Map;
class EntityManager;

#include <memory>

class SharedContext
{
public:
	std::shared_ptr<Window> GetWindow() const
	{
		return m_Window.lock();
	}

	void SetWindow(const std::weak_ptr<Window> window)
	{
		m_Window = window;
	}

	std::shared_ptr<EventManager> GetEventManager() const
	{
		return m_EventManager.lock();
	}

	void SetEventManager(const std::weak_ptr<EventManager> eventManager)
	{
		m_EventManager = eventManager;
	}

	std::shared_ptr<TextureManager> GetTextureManager() const
	{
		return m_TextureManager.lock();
	}

	void SetTextureManager(const std::weak_ptr<TextureManager> textureManager)
	{
		m_TextureManager = textureManager;
	}

	std::shared_ptr<Map> GetMap() const
	{
		return m_Map.lock();
	}

	void SetMap(const std::weak_ptr<Map> map)
	{
		m_Map = map;
	}

	std::shared_ptr<EntityManager> GetEntityManager() const
	{
		return m_EntityManager.lock();
	}

	void SetEntityManager(const std::weak_ptr<EntityManager>& entityManager)
	{
		m_EntityManager = entityManager;
	}

private:

	std::weak_ptr<Window> m_Window;
	std::weak_ptr<EventManager> m_EventManager;
	std::weak_ptr<TextureManager> m_TextureManager;
	std::weak_ptr<Map> m_Map;
	std::weak_ptr<EntityManager> m_EntityManager;
};
