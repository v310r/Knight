#pragma once

#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>
#include "BaseState.h"
#include "StateType.h"
#include "../SharedContext/SharedContext.h"


using StateContainer = std::vector<std::pair<StateType, BaseState*>>;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map<StateType, std::function<BaseState* (void)>>;

class StateManager
{
public:

	StateManager(SharedContext* shared);

	~StateManager();

	void Update(const sf::Time& deltaTime);
	void Draw();

	void ProcessRequests();

	SharedContext* GetContext();
	bool HasState(const StateType type);

	void SwitchTo(const StateType type);
	void Remove(const StateType type);

private:

	void CreateState(const StateType type);
	void RemoveState(const StateType type);

	template<class T>
	void RegisterState(const StateType type)
	{
		m_StateFactory[type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	SharedContext* m_SharedContext;
	StateContainer m_States;
	TypeContainer m_StatesToRemove;
	StateFactory m_StateFactory;
	
};
