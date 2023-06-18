#include "Character.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "Animation/SpriteDirection.h"
#include "Animation/SpriteSheet.h"
#include "Entities/EntityManager.h"
#include "SharedContext/SharedContext.h"
#include "Animation/AnimBase.h"

#ifdef _DEBUG
#include "Utilities/RectangleDebugger.h"



constexpr float ANIMATION_THRESHOLD_BY_VELOCITY_X = 0.1f;
constexpr float ANIMATION_THRESHOLD_BY_VELOCITY_Y = 0.1f;

#else
constexpr float ANIMATION_THRESHOLD_BY_VELOCITY_X = 1.0f;
constexpr float ANIMATION_THRESHOLD_BY_VELOCITY_Y = 1.0f;
#endif


Character::Character(const std::shared_ptr<EntityManager>& entityManager)
	: EntityBase(entityManager)
{
	m_SpriteSheet = std::make_unique<SpriteSheet>(GetEntityManager()->GetContext()->GetTextureManager());
	m_Name = "Character";
}

Character::~Character()
{

}

void Character::Update(float deltaTime)
{
	EntityBase::Update(deltaTime);

	if (m_AttackAABB.width != 0.0f && m_AttackAABB.height != 0.0f)
	{
		UpdateAttackAABB();
	}

	if (m_State != EntityState::Dying && m_State != EntityState::Attacking && m_State != EntityState::Hurt)
	{
		if (std::abs(m_Velocity.y) >= ANIMATION_THRESHOLD_BY_VELOCITY_Y)
		{
			SetState(EntityState::Jumping);
		}
		else if (std::abs(m_Velocity.x) >= ANIMATION_THRESHOLD_BY_VELOCITY_X && m_bCollidingOnY)
		{
			SetState(EntityState::Walking);
		}
		else if (m_bCollidingOnY)
		{
			SetState(EntityState::Idle);
		}
	}
	else if (m_State == EntityState::Attacking || m_State == EntityState::Hurt)
	{
		if (!m_SpriteSheet->GetCurrentAnimation()->IsPlaying())
		{
			SetState(EntityState::Idle);
		}
	}
	else if (m_State == EntityState::Dying)
	{
		if (!m_SpriteSheet->GetCurrentAnimation()->IsPlaying())
		{
			GetEntityManager()->Remove(GetId());
		}
	}

	Animate();
	m_SpriteSheet->Update(deltaTime);
	m_SpriteSheet->SetSpritePosition(m_Position);
}

void Character::Draw(sf::RenderWindow* window)
{
	m_SpriteSheet->Draw(*window);

#ifdef _DEBUG
	RectangleDebugger::SetFillColor(sf::Color(0, 0, 255, 128));
	RectangleDebugger::Draw(window, m_AABB);
	RectangleDebugger::SetFillColor(sf::Color(255, 0, 0, 128));
	RectangleDebugger::Draw(window, m_AttackAABB);

	RectangleDebugger::SetFillColor(sf::Color(255, 255, 0, 128));
	for (CollisionManifold& manifold : m_DebugCollisions)
	{
		RectangleDebugger::Draw(window, manifold.TileBounds);
	}

	m_DebugCollisions.clear();

#endif
}

void Character::Move(const SpriteDirection& direction)
{
	if (m_State == EntityState::Dying)
	{
		return;
	}

	m_SpriteSheet->SetDirection(direction);

	if (direction == SpriteDirection::Left)
	{
		Accelerate(-m_Speed.x, 0.0f);
	}
	else
	{
		Accelerate(m_Speed.x, 0.0f);
	}

	if (m_State == EntityState::Idle)
	{
		SetState(EntityState::Walking);
	}
}

void Character::Jump()
{
	if (m_State == EntityState::Dying || m_State == EntityState::Jumping || m_State == EntityState::Hurt)
	{
		return;
	}

	SetState(EntityState::Jumping);
	AddVelocity(0.0f, -m_JumpVelocity);
}

void Character::Attack()
{
	if (m_State == EntityState::Dying || m_State == EntityState::Jumping || m_State == EntityState::Hurt || m_State == EntityState::Attacking)
	{
		return;
	}

	SetState(EntityState::Attacking);
}

void Character::GetHurt(int damage)
{
	if (m_State == EntityState::Dying || m_State == EntityState::Hurt)
	{
		return;
	}

	m_Health = (m_Health - damage) > 0 ? m_Health - damage : 0;

	if (m_Health)
	{
		SetState(EntityState::Hurt);
	}
	else
	{
		SetState(EntityState::Dying);
	}
}

void Character::Load(const std::string& path)
{
	std::ifstream file;
	file.open(std::filesystem::current_path() / path);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << path << ", src: " << __FILE__ << std::endl;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Name")
		{
			keystream >> m_Name;
		}
		else if (type == "Spritesheet")
		{
			std::string path;
			keystream >> path;
			m_SpriteSheet->LoadSheet(path);
		}
		else if (type == "Health")
		{
			keystream >> m_Health;
		}
		else if (type == "CollisionBox")
		{
			sf::Vector2f boxSize;
			keystream >> boxSize.x >> boxSize.y;
			SetSize(boxSize);
		}
		else if (type == "DamageBox")
		{
			keystream >> m_AttackAABBoffset.x >> m_AttackAABBoffset.y >> m_AttackAABB.width >> m_AttackAABB.height;
		}
		else if (type == "Speed")
		{
			keystream >> m_Speed.x >> m_Speed.y;
		}
		else if (type == "JumpVelocity")
		{
			keystream >> m_JumpVelocity;
		}
		else if (type == "MaxVelocity")
		{
			keystream >> m_MaxVelocity.x >> m_MaxVelocity.y;
		}
		else
		{
			std::cerr << "Unknown type in character file: " << type << ", src: " << __FILE__ << std::endl;
		}
	}
}

void Character::UpdateAttackAABB()
{
	m_AttackAABB.left = (m_SpriteSheet->GetDirection() == SpriteDirection::Left)
		? (m_AABB.left - m_AttackAABB.width) - m_AttackAABBoffset.x
		: (m_AABB.left + m_AttackAABB.width) + m_AttackAABBoffset.x;

	m_AttackAABB.top = m_AABB.top + m_AttackAABBoffset.y;
}

void Character::Animate()
{
	if (m_State == EntityState::Walking && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Walk")
	{
		m_SpriteSheet->SetAnimation("Walk", true, true);
	}
	else if (m_State == EntityState::Jumping && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Jump")
	{
		m_SpriteSheet->SetAnimation("Jump", true, false);
	}
	else if (m_State == EntityState::Attacking && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Attack")
	{
		m_SpriteSheet->SetAnimation("Attack", true, false);
	}
	else if (m_State == EntityState::Hurt && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Hurt")
	{
		m_SpriteSheet->SetAnimation("Hurt", true, false);
	}
	else if (m_State == EntityState::Dying && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Death")
	{
		m_SpriteSheet->SetAnimation("Death", true, false);
	}
	else if (m_State == EntityState::Idle && m_SpriteSheet->GetCurrentAnimation()->GetName() != "Idle")
	{
		m_SpriteSheet->SetAnimation("Idle", true, true);
	}
}
