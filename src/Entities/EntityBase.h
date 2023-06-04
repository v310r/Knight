#pragma once

#include "Utilities/EntityHelper.h"
#include "Utilities/CollisionHelper.h"
#include <SFML/System/Vector2.hpp>
#include <string>


class EntityManager;
namespace sf
{
	class RenderWindow;
}
struct TileInfo;


class EntityBase
{
	friend class EntityManager;

public:
	EntityBase(EntityManager* entityManager);

	virtual ~EntityBase();

	virtual void Update(float deltaTime);

	virtual void Draw(sf::RenderWindow* window) = 0;

	void Move(float x, float y);
	void Move(sf::Vector2f position);

	void AddVelocity(float x, float y);
	void AddVelocity(sf::Vector2f velocity);

	void Accelerate(float x, float y);
	void Accelerate(sf::Vector2f value);
	void SetAcceleration(float x, float y);
	void SetAcceleration(sf::Vector2f value);

	sf::Vector2f GetAcceleration() const { return m_Acceleration; }

	void ApplyFriction(float x, float y);
	void ApplyFriction(sf::Vector2f value);

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f position);

	sf::Vector2f GetPosition() const { return m_Position; }

	void SetVelocity(float x, float y);
	void SetVelocity(sf::Vector2f value);

	sf::Vector2f GetVelocity() const { return m_Velocity; }

	void SetSize(float x, float y);
	void SetSize(sf::Vector2f size);

	void SetState(EntityState state);
	EntityState GetState() const { return m_State; }


	const std::string& GetName() const { return m_Name; }

	EntityType GetType() const { return m_Type; }

	unsigned int GetId() const { return m_ID; }

protected:
	/*
		bAttack - flag to determine whether collision occurred using an attack collision box
	*/
	virtual void OnEntityCollision(EntityBase* collidedEntity, bool bAttack) = 0;

	void UpdateAABB();

	void DetectCollisions();

	void ResolveCollisions();

	std::string m_Name = "BaseEntity";
	EntityType	m_Type = EntityType::Base;
	unsigned int m_ID = 0;

	sf::Vector2f m_Position;
	sf::Vector2f m_PositionOld;
	sf::Vector2f m_Velocity;
	sf::Vector2f m_MaxVelocity;
	sf::Vector2f m_Speed; // Value of acceleration
	sf::Vector2f m_Acceleration;
	sf::Vector2f m_Friction;
	
	TileInfo* m_ReferenceTile = nullptr; // tile underneath entity

	sf::FloatRect m_AABB;
	sf::Vector2f m_Size;

	EntityState m_State = EntityState::Idle;

	bool m_bCollidingOnX = false;
	bool m_bCollidingOnY = false;

	Collisions m_Collisions;
#ifdef _DEBUG
	Collisions m_DebugCollisions;
#endif
	EntityManager* m_EntityManager;

};
