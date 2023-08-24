#pragma once

#include "Utils.h"
#include "Components.h"
#include <memory>

class Entity {

	friend class EntityManager;

private:
	bool m_active = true;
	size_t m_id = 0;
	E_TAG m_tag = E_TAG::DEFAULT;

	Entity(const size_t id, const E_TAG tag)
		: m_id(id), m_tag(tag) {}

public:
	//conponent pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifeSpan> cLifeSpan;


	// private functions
	bool isActive() const;
	E_TAG tag() const;
	const size_t id() const;
	void destory();
};