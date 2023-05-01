#pragma once

#include <SFML/Graphics.hpp>
#include "../Events/EventManager.h"

class StateManager;

class BaseState
{
public:

	BaseState(StateManager* stateManager)
		: m_StateManager(stateManager), m_Transparent(false), m_Transcendent(false)
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

	void SetTransparent(bool value)
	{
		m_Transparent = value;
	}

	bool IsTransparent() const
	{
		return m_Transparent;
	}

	void SetTranscendent(bool value)
	{
		m_Transcendent = value;
	}

	bool IsTranscendent() const
	{
		return m_Transcendent;
	}

	StateManager* GetStateManager()
	{
		return m_StateManager;
	}

	sf::View& GetView() { return m_View; }
	const sf::View& GetView() const { return m_View; }

protected:

	friend class StateManager;

	StateManager* m_StateManager;
	bool m_Transparent;
	bool m_Transcendent;
	sf::View m_View;
};
