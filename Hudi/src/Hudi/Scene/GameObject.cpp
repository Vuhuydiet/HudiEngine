#include "hdpch.h"
#include "GameObject.h"

#include "Hudi/Components/TransformComponent.h"
#include "Hudi/Renderer/SpriteRendererComponent.h"
#include "Hudi/Renderer/RenderSystem.h"

namespace Hudi {

	GameObject::GameObject()
	{
		CreateGameObject();
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

	void GameObject::SetParent(GameObject* parent)
	{
	}

	void GameObject::AddChild(GameObject* child)
	{
	}

}