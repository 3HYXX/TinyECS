# **C++和简易ECS架构的飞机大战**

l 使用C++和SFML图形库进行输入处理和图像绘制，实现了一个简易的ECS架构并在此基础上完成了飞机大战的游戏编写，多个Component附着于一个Entity，System可通过Tag筛选Entity并进行处理



![Game](images\Game.gif)

## ECS优化（未实装）

增加hasComponent,addComponent,getComponent三个函数，用法类似于Unity，更符合习惯

```c++
#pragma once
#include "Utils.h"
#include "Components.h"
#include <memory>

using ComponentTuple = std::tuple<CTransform, CLifeSpan, CInput, CCollision, CScore, CShape>;

class Entity {
	friend class EntityManager;
private:
	bool m_active = true;
	size_t m_id = 0;
	E_TAG m_tag = E_TAG::DEFAULT;
	ComponentTuple m_components;
	
	Entity(const size_t id, const E_TAG tag)
		: m_id(id), m_tag(tag) { }
public:
	template <typename T>
	bool hasComponent() {
		return getComponent<T>().has;
	}
	
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& getComponent() {
		return std::get<T>(m_components);
	}
	// private functions
	bool isActive() const;
	E_TAG tag() const;
	const size_t id() const;
	void destory();
};
```





