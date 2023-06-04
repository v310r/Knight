#include "EntityManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "SharedContext/SharedContext.h"
#include "Window/Window.h"
#include "Player.h"
#include "Enemy.h"


EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities)
	: m_Context(context), m_MaxEntities(maxEntities)
{
	//LoadEnemyTypes("cfg/Entities/EnemyList.list");
	RegisterEntity<Player>(EntityType::Player);
	//RegisterEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{

}

int EntityManager::Add(EntityType type, const std::string& name)
{
	auto iter = m_EntityFactory.find(type);
	if (iter == m_EntityFactory.end())
	{
		return -1;
	}

	const auto& [entityType, entityFactoryFunction] = *iter;

	EntityBase* entity = entityFactoryFunction();
	entity->m_ID = m_IdCounter;
	if (!name.empty())
	{
		entity->m_Name = name;
	}

	m_Entities.emplace(m_IdCounter, entity);
	if (type == EntityType::Enemy)
	{
		auto iter = m_EnemyTypes.find(name);
		if (iter != m_EnemyTypes.end())
		{
			const auto& [enemyName, enemyCharacterFilePath] = *iter;
			Enemy* enemy = static_cast<Enemy*>(entity);
			enemy->Load(enemyCharacterFilePath);
		}
	}

	++m_IdCounter;

	return m_IdCounter - 1;
}

EntityBase* EntityManager::Find(unsigned int id)
{
	auto iter = m_Entities.find(id);
	return (iter != m_Entities.end()) ? iter->second : nullptr;
}

EntityBase* EntityManager::Find(const std::string& name)
{
	for (auto& [id, entity] : m_Entities)
	{
		if (entity->GetName() == name)
		{
			return entity;
		}
	}

	return nullptr;
}

void EntityManager::Remove(unsigned int id)
{
	m_EntitiesToRemove.emplace_back(id);
}

void EntityManager::Update(float deltaTime)
{
	for (auto& [id, entity] : m_Entities)
	{
		entity->Update(deltaTime);
	}

	EntityCollisionProcessing();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow* const window = m_Context->GetWindow()->GetRenderWindow();
	const sf::FloatRect viewSpace = m_Context->GetWindow()->GetViewSpace();

	for (auto& [id, entity] : m_Entities)
	{
		if (!viewSpace.intersects(entity->m_AABB))
		{
			continue;
		}

		entity->Draw(window);
	}
}

void EntityManager::PurgeEntities()
{
	for (auto& [id, entity] : m_Entities)
	{
		delete entity;
		entity = nullptr;
	}

	m_Entities.clear();
	m_IdCounter = 0;
}

void EntityManager::ProcessRemovals()
{
	while (m_EntitiesToRemove.begin() != m_EntitiesToRemove.end())
	{
		unsigned int id = m_EntitiesToRemove.back();
		auto iter = m_Entities.find(id);
		if (iter != m_Entities.end())
		{
			auto& [id, entity] = *iter;

			std::cout << "Removing entity: " << entity->GetId() << ", src: " << __FILE__ << std::endl;

			delete entity;
			entity = nullptr;
			m_Entities.erase(iter);
		}

		m_EntitiesToRemove.pop_back();
	}
}

void EntityManager::LoadEnemyTypes(const std::string& path)
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
		std::string name;
		std::string characterFile;
		keystream >> name >> characterFile;
		m_EnemyTypes.emplace(name, characterFile);
	}
}

void EntityManager::EntityCollisionProcessing()
{
	if (m_Entities.empty())
	{
		return;
	}

	for (auto iterA = m_Entities.begin(); std::next(iterA) != m_Entities.end(); ++iterA)
	{
		auto& [idA, entityA] = *iterA;
		for (auto iterB = std::next(iterA); iterB != m_Entities.end(); ++iterB)
		{
			auto& [idB, entityB] = *iterB;
			if (idA == idB)
			{
				continue;
			}

			if (entityA->m_AABB.intersects(entityB->m_AABB))
			{
				entityA->OnEntityCollision(entityB, false);
				entityB->OnEntityCollision(entityA, false);
			}

			const EntityType typeA = entityA->GetType();
			const EntityType typeB = entityB->GetType();
			if (typeA == EntityType::Player || typeA == EntityType::Enemy)
			{
				Character* characterA = static_cast<Character*>(entityA);
				if (characterA->GetAttackAABB().intersects(entityB->m_AABB))
				{
					characterA->OnEntityCollision(entityB, true);
				}
			}

			if (typeB == EntityType::Player || typeB == EntityType::Enemy)
			{
				Character* characterB = static_cast<Character*>(entityB);
				if (characterB->GetAttackAABB().intersects(entityA->m_AABB))
				{
					characterB->OnEntityCollision(entityA, true);
				}
			}
		}
	}
}

