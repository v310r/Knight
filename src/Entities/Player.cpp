#include "Player.h"

#include "Events/EventManager.h"
#include "Entities/EntityManager.h"
#include "SharedContext/SharedContext.h"
#include "Animation/SpriteSheet.h"
#include "Animation/AnimBase.h"


Player::Player(EntityManager* entityManager) : Character(entityManager)
{
	Load("cfg/Entities/Player.char");
	m_Type = EntityType::Player;

	EventManager* eventManager = m_EntityManager->GetContext()->GetEventManager();
	eventManager->AddCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::MoveLeft, this);
	eventManager->AddCallback<Player>(StateType::Game, "Player_StopLeft", &Player::StopLeft, this);
	eventManager->AddCallback<Player>(StateType::Game, "Player_MoveRight", &Player::MoveRight, this);
	eventManager->AddCallback<Player>(StateType::Game, "Player_StopRight", &Player::StopRight, this);

	eventManager->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::Jump, this);
	eventManager->AddCallback<Player>(StateType::Game, "Player_Attack", &Player::Attack, this);
}

Player::~Player()
{
	EventManager* eventManager = m_EntityManager->GetContext()->GetEventManager();
	eventManager->RemoveCallback(StateType::Game, "Player_MoveLeft");
	eventManager->RemoveCallback(StateType::Game, "Player_MoveRight");
	eventManager->RemoveCallback(StateType::Game, "Player_Jump");
	eventManager->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::Update(float deltaTime)
{
	if (m_bKeyPressedLeft)
	{
		Move(SpriteDirection::Left);
	}
	else if (m_bKeyPressedRight)
	{
		Move(SpriteDirection::Right);
	}

	Character::Update(deltaTime);
}

void Player::OnEntityCollision(EntityBase* collidedEntity, bool bAttack)
{
	if (m_State == EntityState::Dying)
	{
		return;
	}

	if (bAttack)
	{
		if (m_State != EntityState::Attacking)
		{
			return;
		}

		if (!m_SpriteSheet->GetCurrentAnimation()->IsInAction())
		{
			return;
		}

		if (collidedEntity->GetType() != EntityType::Enemy && collidedEntity->GetType() != EntityType::Player)
		{
			return;
		}

		Character* opponent = static_cast<Character*>(collidedEntity);
		opponent->GetHurt(1); // currently hardcoded
		if (m_Position.x > opponent->GetPosition().x)
		{
			opponent->AddVelocity(-32.0f, 0.0f);
		}
		else
		{
			opponent->AddVelocity(32.0f, 0.0f);
		}
	}
	else
	{
		// Some other behavior
	}
}

void Player::MoveRight(EventDetails* details)
{
	m_bKeyPressedRight = true;
}

void Player::StopRight(EventDetails* details)
{
	m_bKeyPressedRight = false;
}

void Player::MoveLeft(EventDetails* details)
{
	m_bKeyPressedLeft = true;
}

void Player::StopLeft(EventDetails* details)
{
	m_bKeyPressedLeft = false;
}

void Player::Jump(EventDetails* details)
{
	Character::Jump();
}

void Player::Attack(EventDetails* details)
{
	Character::Attack();
}
