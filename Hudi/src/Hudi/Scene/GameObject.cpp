#include "hdpch.h"
#include "GameObject.h"

#include <type_traits>

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

	template <typename... Args>
	typename decltype(std::enable_if<sizeof...(Args)==0>::type()) CopyObjectComponents(const GameObject& dst, const GameObject& src) {}

	template <typename T, typename... Args>
	void CopyObjectComponents(const GameObject& dst, const GameObject& src)
	{
		if (src.HasComponent<T>())
		{
			dst.GetOrAddComponent<T>() = src.GetComponent<T>();
		}
		CopyObjectComponents<Args...>(dst, src);
	}

	void GameObject::CopyComponents(const GameObject& srcObj)
	{
		if (!this->IsValid() || !srcObj.IsValid())
		{
			HD_CORE_ERROR("Invalid object being copied!");
			return;
		}

		this->m_ScriptPaths = srcObj.m_ScriptPaths;

		if (srcObj.HasComponent<IDComponent>() && !this->HasComponent<IDComponent>())
			this->AddComponent<IDComponent>();

		CopyObjectComponents<Transform, SpriteRenderer, Camera, Rigidbody2D, BoxCollider2D>(*this, srcObj);
	}

	bool GameObject::operator==(const GameObject& other) const
	{
		return m_Entity == other.m_Entity && world == other.world;
	}

	bool GameObject::operator!=(const GameObject& other) const
	{
		return m_Entity != other.m_Entity || world != other.world;
	}

	template <typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args) const
	{
		T* component = world->AddComponent<T>(m_Entity, args...);
		if (dynamic_cast<Component*>(component))
		{
			Component* base = dynamic_cast<Component*>(component);
			base->m_Entity = this->m_Entity;
			base->world = this->world;
		}
		return *component;
	}

	std::vector<void*> GameObject::GetComponents() const
	{
		return world->GetComponents(m_Entity);
	}

}