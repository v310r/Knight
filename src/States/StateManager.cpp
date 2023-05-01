#include "StateManager.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "PausedState.h"

StateManager::StateManager(SharedContext* shared) : m_SharedContext(shared)
{
	RegisterState<IntroState>(StateType::Intro);
	RegisterState<MainMenuState>(StateType::MainMenu);
	RegisterState<GameState>(StateType::Game);
	RegisterState<PausedState>(StateType::Paused);
}

StateManager::~StateManager()
{
	for (auto& iter : m_States)
	{
		iter.second->OnDestroy();
		delete iter.second;
	}
}

void StateManager::Update(const sf::Time& deltaTime)
{
	if (m_States.empty())
		return;

	if (m_States.back().second->IsTranscendent() && m_States.size() > 1)
	{
		auto i = m_States.end();
		for (; i != m_States.begin(); i--)
		{
			if (i->second->IsTranscendent() == false)
			{
				break;
			}
		}

		for (; i != m_States.end(); i++)
		{
			i->second->Update(deltaTime);
		}
	}
	else
	{
		m_States.back().second->Update(deltaTime);
	}
}

void StateManager::Draw()
{
	if (m_States.empty())
		return;

	if (m_States.back().second->IsTransparent() && m_States.size() > 1)
	{
		auto i = m_States.end() - 1;
		for (; i != m_States.begin(); i--)
		{
			if (i->second->IsTransparent() == false)
			{
				break;
			}
		}

		for (; i != m_States.end(); i++)
		{
			m_SharedContext->GetWindow()->GetRenderWindow()->setView(i->second->GetView());
			i->second->Draw();
		}
	}
	else
	{
		m_States.back().second->Draw();
	}
}

void StateManager::ProcessRequests()
{
	while (m_StatesToRemove.begin() != m_StatesToRemove.end())
	{
		RemoveState(*m_StatesToRemove.begin());
		m_StatesToRemove.erase(m_StatesToRemove.begin());
	}
}

SharedContext* StateManager::GetContext()
{
	return m_SharedContext;
}

bool StateManager::HasState(const StateType type)
{
	for (auto i = m_States.begin(); i != m_States.end(); i++)
	{
		if (i->first == type)
		{
			auto removed = std::find(m_StatesToRemove.begin(), m_StatesToRemove.end(), type);
			if (removed == m_StatesToRemove.end())
				return true;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType type)
{
	m_SharedContext->GetEventManager()->SetCurrentState(type);
	for (auto i = m_States.begin(); i != m_States.end(); i++)
	{
		if (i->first == type)
		{
			m_States.back().second->Deactivate();
			StateType tmpType = i->first;
			BaseState* tmpState = i->second;
			m_States.erase(i);
			m_States.emplace_back(tmpType, tmpState);
			tmpState->Activate();
			m_SharedContext->GetWindow()->GetRenderWindow()->setView(tmpState->GetView());
			return;
		}
	}

	// in case state wasn't found
	BaseState* const lastState = m_States.back().second;
	if (m_States.empty() == false)
	{
		lastState->Deactivate();
	}
	CreateState(type);
	lastState->Activate();
	m_SharedContext->GetWindow()->GetRenderWindow()->setView(lastState->GetView());
}

void StateManager::Remove(const StateType type)
{
	m_StatesToRemove.push_back(type);
}

void StateManager::CreateState(const StateType type)
{
	auto newState = m_StateFactory.find(type);
	if (newState == m_StateFactory.end())
		return;

	BaseState* state = newState->second();
	state->m_View = m_SharedContext->GetWindow()->GetRenderWindow()->getDefaultView();
	m_States.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType type)
{
	for (auto i = m_States.begin(); i != m_States.end(); i++)
	{
		if (i->first == type)
		{
			i->second->OnDestroy();
			delete i->second;
			m_States.erase(i);
			return;
		}
	}
}