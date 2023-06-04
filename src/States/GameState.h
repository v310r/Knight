#pragma once

#include "BaseState.h"

class Map;

class GameState : public BaseState
{
public:

	GameState(StateManager* stateManager);

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void Update(const float deltaTime) override;
	virtual void Draw() override;

	void GoToMainMenu(EventDetails* details);
	void Pause(EventDetails* details);

private:
	Map* m_Map;
};

