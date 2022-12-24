#pragma once

#include <SFML/Graphics.hpp>
#include "../Events/EventManager.h"

class StateManager;

class BaseState
{
public:

	BaseState(StateManager* stateManager)
		: m_stateManager(stateManager), m_transparent(false), m_transcendent(false)
	{

	}

	virtual ~BaseState()
	{

	}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& deltaTime) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool transparent)
	{
		m_transparent = transparent;
	}

	bool IsTransparent() const
	{
		return m_transparent;
	}

	void SetTranscendent(const bool transcendent)
	{
		m_transcendent = transcendent;
	}

	bool IsTranscendent() const
	{
		return m_transcendent;
	}

	StateManager* GetStateManager()
	{
		return m_stateManager;
	}

protected:

	friend class StateManager;

	StateManager* m_stateManager;
	bool m_transparent;
	bool m_transcendent;
};
