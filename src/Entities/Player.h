#pragma once

#include "Character.h"


class EntityManager;
struct EventDetails;


class Player : public Character
{
public:
	Player(EntityManager* entityManager);

	~Player();

	virtual void OnEntityCollision(EntityBase* collidedEntity, bool bAttack) override;

	void MoveRight(EventDetails* details);

	void MoveLeft(EventDetails* details);

	void Jump(EventDetails* details);

	void Attack(EventDetails* details);
};

