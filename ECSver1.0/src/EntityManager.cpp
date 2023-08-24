#include "EntityManager.h"
#include <algorithm>


std::shared_ptr<Entity> EntityManager::addEntity(E_TAG tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_toAdd.push_back(e);
	return e;
}

EntityVec& EntityManager::getEntities()
{
	return this->m_entities;
}

EntityVec& EntityManager::getEntities(E_TAG tag)
{
	return this->m_entityMap[tag];
}

void EntityManager::Update()
{
	for (auto e : m_toAdd) {
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	for (auto e = m_entities.begin(); e != m_entities.end();) {
		if (!(*e)->isActive()) {
			auto iter = std::remove(m_entityMap[(*e)->tag()].begin(), m_entityMap[(*e)->tag()].end(), (*e));
			m_entityMap[(*e)->tag()].erase(iter, m_entityMap[(*e)->tag()].end());
			e = m_entities.erase(e);
		}
		else {
			e++;
		}
	}
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e) {return e->isActive() == false; }), m_entities.end());
	m_toAdd.clear();
}
