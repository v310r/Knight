#pragma once

#include "Character.h"


class EntityManager;
struct EventDetails;


class Player : public Character
{
public:
	Player(const std::shared_ptr<EntityManager>& entityManager);

	~Player();

	virtual void Update(float deltaTime) override;

	virtual void OnEntityCollision(EntityBase* collidedEntity, bool bAttack) override;

	void MoveRight(EventDetails* details);

	void StopRight(EventDetails* details);

	void MoveLeft(EventDetails* details);

	void StopLeft(EventDetails* details);

	void Jump(EventDetails* details);

	void Attack(EventDetails* details);

private:
	bool m_bKeyPressedLeft = false;
	bool m_bKeyPressedRight = false;
};

