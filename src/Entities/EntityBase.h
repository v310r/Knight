#pragma once

#include "../Utilities/EntityHelper.h"
#include "../Utilities/CollisionHelper.h"
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

	void ApplyFriction(float x, float y);
	void ApplyFriction(sf::Vector2f value);

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f position);

	void SetVelocity(float x, float y);
	void SetVelocity(sf::Vector2f value);

	void SetSize(float x, float y);
	void SetSize(sf::Vector2f size);

	void SetState(EntityState state);
	
protected:
	virtual void OnEntityCollision(EntityBase* collider, bool bAttack) = 0;

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
	EntityManager* m_EntityManager;

};
