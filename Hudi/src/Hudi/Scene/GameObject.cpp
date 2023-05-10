#include "hdpch.h"
#include "GameObject.h"

namespace Hudi {

	GameObject::GameObject()
		: m_Entity(0), world(nullptr)
	{
	}

	GameObject::GameObject(ECS::World* _world)
		: world(_world), m_Entity(0)
	{
		if (world)
			m_Entity = world->CreateEntity();
	}

	void GameObject::Destroy()
	{
		world->DestroyEntity(m_Entity);
		m_Entity = 0;
		world = nullptr;
	}

	void GameObject::Reset()
	{
		m_Entity = 0;
		world = nullptr;
	}

	bool GameObject::operator==(const GameObject& other)
	{
		return m_Entity == other.m_Entity && world == other.world;
	}

	bool GameObject::operator!=(const GameObject& other)
	{
		return m_Entity != other.m_Entity || world != other.world;
	}

	std::vector<Ref<Component>> GameObject::GetComponents()
	{
		std::vector<Ref<Component>> res;
		for (auto& comp : world->GetComponents(m_Entity))
		{
			res.push_back(std::static_pointer_cast<Component>(comp));
		}
		return res;
	}

}