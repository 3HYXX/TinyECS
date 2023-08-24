#pragma once

#include <memory>
#include "Entity.h"
#include "Utils.h"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<E_TAG, EntityVec>;

class EntityManager {
private:
	EntityVec m_entities;
	EntityVec m_toAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;
public:
	EntityManager() = default;
	std::shared_ptr<Entity> addEntity(E_TAG tag);
	EntityVec& getEntities();
	EntityVec& getEntities(E_TAG tag);
	void Update();

};