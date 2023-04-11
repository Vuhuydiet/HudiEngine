#include "hdpch.h"
#include "Scene.h"

#include "CameraComponent.h"

namespace Hudi {

	Scene::Scene(uint8_t index)
		: m_BuildIndex(index)
	{
		m_World = NewRef<ECS::World>();
	}

	void Scene::BeginScene()
	{
		HD_CORE_INFO("Start scene {0}", m_BuildIndex);

		/*GameObject& mainCamera = CreateGameObject("Main Camera");
		mainCamera.AddComponent<Camera>();
		m_ActiveCamera = &mainCamera;*/

	}

	void Scene::EndScene()
	{
		HD_CORE_INFO("End scene {0}", m_BuildIndex);
		m_GameObjects.clear();
		m_EntityToName.clear();
		m_NameToEntity.clear();
	}

	void Scene::OnUpdate(float dt)
	{
		m_World->Each<Component>(&Component::Awake);
		m_World->Each<Component>(&Component::Update);
	}

	Ref<GameObject> Scene::CreateGameObject(const std::string& _name)
	{
		std::string name = _name;

		static uint8_t ind = 1;
		if (m_NameToEntity.find(name) != m_NameToEntity.end())
			name += std::to_string(ind++);
		
		Ref<GameObject> obj = NewRef<GameObject>(m_World);
		uint32_t id = obj->GetEntityID();
		m_GameObjects[id] = obj;
		m_EntityToName[id] = name;
		m_NameToEntity[name] = id;
		return obj;
	}

	bool Scene::HasGameObject(const std::string& _name)
	{
		return m_NameToEntity.find(_name) != m_NameToEntity.end();
	}

	bool Scene::HasGameObject(uint32_t id)
	{
		return m_EntityToName.find(id) != m_EntityToName.end();
	}

	Ref<GameObject> Scene::GetGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named \"{0}\" does not exist!", _name);
			return nullptr;
		}

		return m_GameObjects[m_NameToEntity[_name]];
	}

	Ref<GameObject> Scene::GetGameObject(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Game Object with ID \"{0}\" does not exist!", id);
			return nullptr;
		}

		return m_GameObjects[id];
	}

	void Scene::DestroyGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("GameObject named \"{0}\" does not exist in the scene!", _name);
			return;
		}

		uint32_t id = m_NameToEntity[_name];
		Ref<GameObject> go = m_GameObjects[m_NameToEntity[_name]];
		if (go->GetParent())
		{
			go->GetParent()->RemoveChild(go);
		}
		for (auto& child : go->GetChildren())
		{
			go->RemoveChild(child);
			DestroyGameObject((ECS::Entity)*child);
		}

		m_GameObjects.erase(id);
		m_EntityToName.erase(id);
		m_NameToEntity.erase(_name);
	}

	void Scene::DestroyGameObject(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Destroyed a GameObject that does not exist!");
			return;
		}

		Ref<GameObject> go = m_GameObjects[id];
		std::string name = m_EntityToName[id];
		if (go->GetParent())
		{
			go->GetParent()->RemoveChild(go);
		}
		for (auto& child : go->GetChildren())
		{
			DestroyGameObject((ECS::Entity)*child);
			go->RemoveChild(child);
		}

		m_GameObjects.erase(id);
		m_EntityToName.erase(id);
		m_NameToEntity.erase(name);
	}

	std::string Scene::GetGameObjectName(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Get the name of a GameObject that does not exist!");
			return "unknown";
		}

		return m_EntityToName[id];
	}

	void Scene::RenameGameObject(const std::string& _name, uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_ERROR("Renamed GameObject with ID \"{0}\" that does not exist!", id);
			return;
		}

		std::string old_name = m_EntityToName[id];
		m_EntityToName[id] = _name;
		m_NameToEntity.erase(old_name);
		m_NameToEntity[_name] = id;
	}

	void Scene::SetActiveCamera(Ref<GameObject> cam)
	{
		if (!cam->HasComponent<Camera>())
		{
			HD_CORE_ERROR("Set the object that is not a camera gameobject to be the camera object.");
			return;
		}
		m_ActiveCamera = cam;
	}

	const Ref<GameObject> Scene::GetActiveCamera()
	{
		if (!m_ActiveCamera)
		{
			HD_CORE_ERROR("No active camera!");
			return nullptr;
		}
		return m_ActiveCamera;
	}

}