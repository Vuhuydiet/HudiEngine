#include "hdpch.h"
#include "GameObject.h"

#include "Hudi/Components/TransformComponent.h"
#include "Hudi/Components/SpriteRendererComponent.h"
#include "Hudi/Renderer/RenderSystem.h"

namespace Hudi {

	GameObject::GameObject()
	{
		CreateGameObject();
		m_Parent = nullptr;
	}

	GameObject::~GameObject()
	{
		DestroyGameObject();
	}

	void GameObject::CreateGameObject()
	{
		m_Entt = ECS::Coordinator::CreateEntity();
		
		AddComponent<Transform>();
		AddComponent<SpriteRenderer>();
	}

	void GameObject::DestroyGameObject()
	{
		ECS::Coordinator::DestroyEntity(m_Entt);
		m_Entt = 0;
	}

	void GameObject::SetParent(Ref<GameObject> parent)
	{
		m_Parent = parent;
	}

	void GameObject::AddChild(Ref<GameObject> child)
	{
		m_Children.push_back(child);
	}

	void GameObject::RemoveChild(Ref<GameObject> child)
	{
		for (auto it = m_Children.begin(); it!=m_Children.end(); it++)
		{
			if (*it == child)
			{
				for (auto it2 = it + 1; it2 != m_Children.end(); it2++)
				{
					*(it2 - 1) = *it2;
				}
				child->GetParent() = nullptr;
				m_Children.pop_back();
				return;
			}
			HD_CORE_WARN("Game Object does not have this child!");
		}
	}

	Ref<GameObject>& GameObject::GetParent()
	{
		return m_Parent;
	}

	std::vector<Ref<GameObject>>& GameObject::GetChildren()
	{
		return m_Children;
	}

}