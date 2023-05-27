#include "EntityManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "EntityBase.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "SharedContext/SharedContext.h"
#include "Window/Window.h"


EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities)
	: m_Context(context), m_MaxEntities(maxEntities)
{
	LoadEnemyTypes("cfg/Entities/EnemyList.list");
	//RegisterEntity<Player>(EntityType::Player);
	//RegisterEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{
	PurgeEntities();
}

int EntityManager::Add(EntityType type, const std::string& name)
{
	auto iter = m_EntityFactory.find(type);
	if (iter == m_EntityFactory.end())
	{
		return -1;
	}

	EntityBase* entity = iter->second();
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
			//Enemy* enemy = static_cast<Enemy*>(entity);
			//enemy->Load(iter->second); // pass file path
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
	for (auto& iter : m_Entities)
	{
		if (iter.second->GetName() == name)
		{
			return iter.second;
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
	for (auto& iter : m_Entities)
	{
		iter.second->Update(deltaTime);
	}

	EntityCollisionProcessing();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow* const window = m_Context->GetWindow()->GetRenderWindow();
	const sf::FloatRect viewSpace = m_Context->GetWindow()->GetViewSpace();

	for (auto& iter : m_Entities)
	{
		if (!viewSpace.intersects(iter.second->m_AABB))
		{
			continue;
		}

		iter.second->Draw(window);
	}
}

void EntityManager::PurgeEntities()
{
	for (auto& iter : m_Entities)
	{
		delete iter.second;
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
			std::cout << "Removing entity: " << iter->second->GetId() << std::endl;
			delete iter->second;
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
		std::cerr << "Failed to open file: " << path << std::endl;
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
		for (auto iterB = std::next(iterA); iterB != m_Entities.end(); ++iterB)
		{
			if (iterA->first == iterB->first)
			{
				continue;
			}

			if (iterA->second->m_AABB.intersects(iterB->second->m_AABB))
			{
				iterA->second->OnEntityCollision(iterB->second, false);
				iterB->second->OnEntityCollision(iterA->second, false);
			}

			//const EntityType typeA = iterA->second->GetType();
			//const EntityType typeB = iterB->second->GetType();
			//if (typeA == EntityType::Player || typeA == EntityType::Enemy)
			//{
			//	Character* characterA = static_cast<Character*>(iterA->second);
			//	if (characterA->GetAttackAABB().intersects(iterB->second->m_AABB))
			//	{
			//		characterA->OnEntityCollision(iterB->second, true);
			//	}
			//}

			//if (typeB == EntityType::Player || typeB == EntityType::Enemy)
			//{
			//	Character* characterB = static_cast<Character*>(iterB->second);
			//	if (characterB->GetAttackAABB().intersects(iterA->second->m_AABB))
			//	{
			//		characterB->OnEntityCollision(iterA->second, true);
			//	}
			//}
		}
	}
}

