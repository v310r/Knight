#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "Utilities/EntityHelper.h"


class EntityBase;
class SharedContext;

using EntityContainer = std::unordered_map<unsigned int, EntityBase*>;
using EntityFactory = std::unordered_map<EntityType, std::function<EntityBase* (void)>>;
using EnemyTypes = std::unordered_map<std::string, std::string>; // (name, characterFile)

class EntityManager
{
public:
	EntityManager(SharedContext* context, unsigned int maxEntities);

	~EntityManager();
	
	int Add(EntityType type, const std::string& name = "");

	EntityBase* Find(unsigned int id);
	EntityBase* Find(const std::string& name);

	void Remove(unsigned int id);

	void Update(float deltaTime);

	void Draw();

	void PurgeEntities();

	SharedContext* GetContext() const { return m_Context; };

private:
	template<class T>
	void RegisterEntity(EntityType type)
	{
		m_EntityFactory[type] = [this]() -> EntityBase*
		{
			return new T(this);
		};
	}

	void ProcessRemovals();

	void LoadEnemyTypes(const std::string& path);

	void EntityCollisionProcessing();

	EntityContainer m_Entities;
	EnemyTypes m_EnemyTypes;
	EntityFactory m_EntityFactory;

	SharedContext* m_Context;

	unsigned int m_IdCounter = 0;
	unsigned int m_MaxEntities;

	std::vector<unsigned int> m_EntitiesToRemove;
};
