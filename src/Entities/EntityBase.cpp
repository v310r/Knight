#include "EntityBase.h"

#include <SFML/Graphics.hpp>
#include "World/Map.h"
#include "Utilities/CollisionHelper.h"
#include "EntityManager.h"
#include <algorithm>
#include <iostream>

EntityBase::EntityBase(const std::shared_ptr<EntityManager>& entityManager) : m_EntityManager(entityManager)
{

}

EntityBase::~EntityBase()
{

}

void EntityBase::Update(float deltaTime)
{

	const std::shared_ptr<Map> map = GetEntityManager()->GetContext()->GetMap();
	const float gravity = map->GetGravity();
	Accelerate(0.0f, gravity);
	AddVelocity(m_Acceleration * deltaTime);
	//std::cout << "Position-> (" << m_Position.x << ", " << m_Position.y << "); Velocity-> (" << m_Velocity.x << ", " << m_Velocity.y << "); -> (Acceleration " << m_Acceleration.x << ", " << m_Acceleration.y << ")\n";
	SetAcceleration(0.0f, 0.0f);

	sf::Vector2f frictionValue;
	if (m_ReferenceTile)
	{
		frictionValue = m_ReferenceTile->Friction;
		if (m_ReferenceTile->bDeadly)
		{
			SetState(EntityState::Dying);
		}
		else if (const TileInfo* const DefaultTile = map->GetDefaultTile())
		{
			frictionValue = DefaultTile->Friction;
		}
		else
		{
			frictionValue = m_Friction;
		}
	}

	const float ResultingFrictionX = (m_Speed.x * frictionValue.x) * deltaTime;
	const float ResultingFrictionY = (m_Speed.y * frictionValue.y) * deltaTime;
	ApplyFriction(ResultingFrictionX, ResultingFrictionY);

	const sf::Vector2f newPosition = m_Velocity * deltaTime;
	Move(newPosition);

	m_bCollidingOnX = false;
	m_bCollidingOnY = false;

	DetectCollisions();
#ifdef _DEBUG
	m_DebugCollisions = m_Collisions;
#endif
	ResolveCollisions();
}

void EntityBase::Move(float x, float y)
{
	Move(sf::Vector2f(x, y));
}

void EntityBase::Move(sf::Vector2f position)
{
	m_PositionOld = m_Position;
	m_Position += position;

	const sf::Vector2u mapSize = GetEntityManager()->GetContext()->GetMap()->GetMapSize();
	const sf::Vector2u UnitMapSize = mapSize * static_cast<unsigned int>(TileSheet::TileSize);

	const sf::Vector2f bodyOffset = m_Size / 2.0f;
	if (m_Position.x - bodyOffset.x < 0.0f)
	{
		m_Position.x = 0.0f + bodyOffset.x;
	}
	else if (m_Position.x + bodyOffset.x > UnitMapSize.x)
	{
		m_Position.x = static_cast<float>(UnitMapSize.x) - bodyOffset.x;
	}

	if (m_Position.y < 0.0f)
	{
		m_Position.y = 0.0f;
	}
	else if (m_Position.y > UnitMapSize.y)
	{
		m_Position.y = static_cast<float>(UnitMapSize.y);
		SetState(EntityState::Dying);
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	AddVelocity(sf::Vector2f(x, y));
}

void EntityBase::AddVelocity(sf::Vector2f velocity)
{
	m_Velocity += velocity;
	if (std::abs(m_Velocity.x) > m_MaxVelocity.x)
	{
		if (m_Velocity.x < 0.0f)
		{
			m_Velocity.x = -m_MaxVelocity.x;
		}
		else
		{
			m_Velocity.x = m_MaxVelocity.x;
		}
	}

	if (std::abs(m_Velocity.y) > m_MaxVelocity.y)
	{
		if (m_Velocity.y < 0.0f)
		{
			m_Velocity.y = -m_MaxVelocity.y;
		}
		else
		{
			m_Velocity.y = m_MaxVelocity.y;
		}
	}
}

void EntityBase::Accelerate(float x, float y)
{
	Accelerate(sf::Vector2f(x, y));
}

void EntityBase::Accelerate(sf::Vector2f value)
{
	m_Acceleration += value;
}

void EntityBase::SetAcceleration(float x, float y)
{
	m_Acceleration.x = x;
	m_Acceleration.y = y;
}

void EntityBase::SetAcceleration(sf::Vector2f value)
{
	SetAcceleration(value.x, value.y);
}

void EntityBase::ApplyFriction(float x, float y)
{
	if (m_Velocity.x != 0)
	{
		if ((std::abs(m_Velocity.x) - std::abs(x)) < 0.0f)
		{
			m_Velocity.x = 0.0f;
		}
		else
		{
			if (m_Velocity.x < 0.0f)
			{
				m_Velocity.x += x;
			}
			else
			{
				m_Velocity.x -= x;
			}
		}
	}

	if (m_Velocity.y != 0)
	{
		if ((std::abs(m_Velocity.y) - std::abs(y)) < 0.0f)
		{
			m_Velocity.y = 0.0f;
		}
		else
		{
			if (m_Velocity.y < 0.0f)
			{
				m_Velocity.y += y;
			}
			else
			{
				m_Velocity.y -= y;
			}
		}
	}
}

void EntityBase::ApplyFriction(sf::Vector2f value)
{
	ApplyFriction(value.x, value.y);
}

void EntityBase::SetPosition(float x, float y)
{
	m_Position = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetPosition(sf::Vector2f position)
{
	m_Position = position;
	UpdateAABB();
}

void EntityBase::SetVelocity(float x, float y)
{
	m_Velocity = { x, y };
}

void EntityBase::SetVelocity(sf::Vector2f value)
{
	m_Velocity = value;
}

void EntityBase::SetSize(float x, float y)
{
	m_Size = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetSize(sf::Vector2f size)
{
	m_Size = size;
	UpdateAABB();
}

void EntityBase::SetState(EntityState state)
{
	if (m_State == EntityState::Dying)
	{
		return;
	}

	m_State = state;
}

void EntityBase::UpdateAABB()
{
	m_AABB = sf::FloatRect(m_Position.x - (m_Size.x / 2.0f), m_Position.y - (m_Size.y / 2.0f), m_Size.x, m_Size.y);
}

void EntityBase::DetectCollisions()
{
	const std::shared_ptr<Map> map = GetEntityManager()->GetContext()->GetMap();
	const float tileSize = static_cast<float>(map->GetTileSize());
	
	const int startX = static_cast<int>(std::floor(m_AABB.left / tileSize));
	const int endX = static_cast<int>(std::floor((m_AABB.left + m_AABB.width) / tileSize));
	const int startY = static_cast<int>(std::floor(m_AABB.top / tileSize));
	const int endY = static_cast<int>(std::floor((m_AABB.top + m_AABB.height) / tileSize));

	for (int x = startX; x <= endX; ++x)
	{
		for (int y = startY; y <= endY; ++y)
		{
			Tile* tile = map->GetTile(x, y);
			if (!tile)
			{
				continue;
			}

			sf::FloatRect intersection;
			if (!m_AABB.intersects(tile->AABB, intersection))
			{
				continue;
			}

			const float area = intersection.width * intersection.height;
			m_Collisions.emplace_back(CollisionManifold(area, tile->Properties, tile->AABB));
			if (tile->bWarp && m_Type == EntityType::Player)
			{
				map->LoadNext();
			}

		}
	}
}

void EntityBase::ResolveCollisions()
{
	if (m_Collisions.empty())
	{

		// Might be unnecessary. Check it later on
		if (!m_bCollidingOnY) 
		{ 
			m_ReferenceTile = nullptr; 
		}

		return;
	}

	std::sort(m_Collisions.begin(), m_Collisions.end(), [](const CollisionManifold& c1, const CollisionManifold c2) 
	{
		return c1.Area > c2.Area;
	});

	const std::shared_ptr<Map> map = GetEntityManager()->GetContext()->GetMap();
	unsigned int tileSize = map->GetTileSize();
	for (auto& manifold : m_Collisions)
	{

		// in case if an entity was moved by collision resolution
		if (!m_AABB.intersects(manifold.TileBounds))
		{
			continue;
		}

		const float xDiff = (m_AABB.left + (m_AABB.width / 2.0f)) - (manifold.TileBounds.left + (manifold.TileBounds.width / 2.0f));
		const float yDiff = (m_AABB.top + (m_AABB.height / 2.0f)) - (manifold.TileBounds.top + (manifold.TileBounds.height / 2.0f));

		float resolve = 0;
		if (std::abs(xDiff) > std::abs(yDiff))
		{
			if (xDiff > 0.0f)
			{
				resolve = (manifold.TileBounds.left + tileSize) - m_AABB.left;
			}
			else
			{
				resolve = -((m_AABB.left + m_AABB.width) - manifold.TileBounds.left);
			}

			Move(resolve, 0.0f);
			m_Velocity.x = 0.0f;
			m_bCollidingOnX = true;
		}
		else
		{
			if (yDiff > 0.0f)
			{
				resolve = (manifold.TileBounds.top + tileSize) - m_AABB.top;
			}
			else
			{
				resolve = -((m_AABB.top + m_AABB.height) - manifold.TileBounds.top);
			}

			Move(0.0f, resolve);
			m_Velocity.y = 0.0f;
			if (m_bCollidingOnY)
			{
				continue;
			}

			m_ReferenceTile = manifold.TileInfo;
			m_bCollidingOnY = true;
		}
	}

	m_Collisions.clear();
}

