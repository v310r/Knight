#include "StateManager.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "PausedState.h"

StateManager::StateManager(SharedContext* shared) : m_context(shared)
{
	RegisterState<IntroState>(StateType::Intro);
	RegisterState<MainMenuState>(StateType::MainMenu);
	RegisterState<GameState>(StateType::Game);
	RegisterState<PausedState>(StateType::Paused);
}

StateManager::~StateManager()
{
	for (auto& iter : m_states)
	{
		iter.second->OnDestroy();
		delete iter.second;
	}
}

void StateManager::Update(const sf::Time& deltaTime)
{
	if (m_states.empty())
		return;

	if (m_states.back().second->IsTranscendent() && m_states.size() > 1)
	{
		auto i = m_states.end();
		for (; i != m_states.begin(); i--)
		{
			if (i->second->IsTranscendent() == false)
			{
				break;
			}
		}

		for (; i != m_states.end(); i++)
		{
			i->second->Update(deltaTime);
		}
	}
	else
	{
		m_states.back().second->Update(deltaTime);
	}
}

void StateManager::Draw()
{
	if (m_states.empty())
		return;

	if (m_states.back().second->IsTransparent() && m_states.size() > 1)
	{
		auto i = m_states.end() - 1;
		for (; i != m_states.begin(); i--)
		{
			if (i->second->IsTransparent() == false)
			{
				break;
			}
		}

		for (; i != m_states.end(); i++)
		{
			i->second->Draw();
		}
	}
	else
	{
		m_states.back().second->Draw();
	}
}

void StateManager::ProcessRequests()
{
	while (m_statesToRemove.begin() != m_statesToRemove.end())
	{
		RemoveState(*m_statesToRemove.begin());
		m_statesToRemove.erase(m_statesToRemove.begin());
	}
}

SharedContext* StateManager::GetContext()
{
	return m_context;
}

bool StateManager::HasState(const StateType type)
{
	for (auto i = m_states.begin(); i != m_states.end(); i++)
	{
		if (i->first == type)
		{
			auto removed = std::find(m_statesToRemove.begin(), m_statesToRemove.end(), type);
			if (removed == m_statesToRemove.end())
				return true;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType type)
{
	m_context->GetEventManager()->SetCurrentState(type);
	for (auto i = m_states.begin(); i != m_states.end(); i++)
	{
		if (i->first == type)
		{
			m_states.back().second->Deactivate();
			StateType tmpType = i->first;
			BaseState* tmpState = i->second;
			m_states.erase(i);
			m_states.emplace_back(tmpType, tmpState);
			tmpState->Activate();
			return;
		}
	}

	// in case state wasn't found
	if (m_states.empty() == false)
	{
		m_states.back().second->Deactivate();
	}
	CreateState(type);
	m_states.back().second->Activate();
}

void StateManager::Remove(const StateType type)
{
	m_statesToRemove.push_back(type);
}

void StateManager::CreateState(const StateType type)
{
	auto newState = m_stateFactory.find(type);
	if (newState == m_stateFactory.end())
		return;

	BaseState* state = newState->second();
	m_states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType type)
{
	for (auto i = m_states.begin(); i != m_states.end(); i++)
	{
		if (i->first == type)
		{
			i->second->OnDestroy();
			delete i->second;
			m_states.erase(i);
			return;
		}
	}
}