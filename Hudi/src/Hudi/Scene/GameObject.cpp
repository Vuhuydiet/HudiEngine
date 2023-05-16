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

	void GameObject::Copy(const GameObject& srcObj)
	{
		if (!this->Valid() || !srcObj.Valid())
		{
			HD_CORE_ERROR("Invalid object being copied!");
			return;
		}

		this->m_ScriptPaths = srcObj.m_ScriptPaths;

		if (srcObj.HasComponent<Transform>())
		{
			this->GetOrAddComponent<Transform>() = srcObj.GetComponent<Transform>();
		}
		if (srcObj.HasComponent<SpriteRenderer>())
		{
			this->GetOrAddComponent<SpriteRenderer>() = srcObj.GetComponent<SpriteRenderer>();
		}
		if (srcObj.HasComponent<Camera>())
		{
			this->GetOrAddComponent<Camera>() = srcObj.GetComponent<Camera>();
		}
		if (srcObj.HasComponent<Rigidbody2D>())
		{
			this->GetOrAddComponent<Rigidbody2D>() = srcObj.GetComponent<Rigidbody2D>();
		}
		if (srcObj.HasComponent<BoxCollider2D>())
		{
			this->GetOrAddComponent<BoxCollider2D>() = srcObj.GetComponent<BoxCollider2D>();
		}
	}

	bool GameObject::operator==(const GameObject& other) const
	{
		return m_Entity == other.m_Entity && world == other.world;
	}

	bool GameObject::operator!=(const GameObject& other) const
	{
		return m_Entity != other.m_Entity || world != other.world;
	}

	std::vector<Ref<Component>> GameObject::GetComponents() const
	{
		std::vector<Ref<Component>> res;
		for (auto& comp : world->GetComponents(m_Entity))
		{
			res.push_back(std::static_pointer_cast<Component>(comp));
		}
		return res;
	}

}