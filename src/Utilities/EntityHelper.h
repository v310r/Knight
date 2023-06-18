#pragma once


enum class EntityType
{
	Base,
	Enemy,
	Player
};

enum class EntityState
{
	Idle,
	Walking,
	Jumping,
	Attacking,
	Hurt,
	Dying
};
