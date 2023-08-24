#include "Entity.h"

bool Entity::isActive() const
{
	return m_active;
}

E_TAG Entity::tag() const
{
	return m_tag;
}

const size_t Entity::id() const
{
	return m_id;
}

void Entity::destory()
{
	this->m_active = false;
}
