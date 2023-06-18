#pragma once

// Global state is usable throughout whole application,
// you can not switch to it, because it is already permanent.
// Trying to add global state might lead to errors!!!

enum class StateType
{
	Global = 0,
	Intro,
	MainMenu,
	Game,
	Paused,
	GameOver,
	Credits
};
