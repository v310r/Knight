#pragma once

#include "EntityBase.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>


namespace sf
{
	class RenderWindow;
}
class SpriteSheet;
enum class SpriteDirection;

class Character : public EntityBase
{
	friend class EntityManager;

public:
	Character(EntityManager* entityManager);

	virtual ~Character();

	virtual void Update(float deltaTime) override;
	
	virtual void Draw(sf::RenderWindow* window) override;

	virtual void OnEntityCollision(EntityBase* collidedEntity, bool attack) = 0;

	void Move(const SpriteDirection& direction);

	void Jump();

	void Attack();

	void GetHurt(int damage);

	void Load(const std::string& path);

protected:
	void UpdateAttackAABB();

	void Animate();

	std::unique_ptr<SpriteSheet> m_SpriteSheet;

	float m_JumpVelocity = 250.0f;

	int m_Health = 5;
	
	sf::FloatRect m_AttackAABB;

	sf::Vector2f m_AttackAABBoffset;
};