#include "hdpch.h"
#include "Scene.h"

#include "CameraComponent.h"

namespace Hudi {

	Scene::Scene()
	{
		static int index = 1;
		m_BuildIndex = index++;
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

	GameObject& Scene::CreateGameObject(const std::string& _name)
	{
		std::string name = _name;

		static uint8_t ind = 1;
		if (m_NameToEntity.find(name) != m_NameToEntity.end())
			name += std::to_string(ind++);
		
		Ref<GameObject> obj = NewRef<GameObject>();
		uint32_t id = (ECS::Entity)*obj;
		m_GameObjects[id] = obj;
		m_EntityToName[id] = name;
		m_NameToEntity[name] = id;
		return *obj;
	}

	bool Scene::HasGameObject(const std::string& _name)
	{
		return m_NameToEntity.find(_name) != m_NameToEntity.end();
	}

	bool Scene::HasGameObject(uint32_t id)
	{
		return m_EntityToName.find(id) != m_EntityToName.end();
	}

	GameObject& Scene::GetGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named \"{0}\" does not exist!", _name);
			static GameObject error;
			return error;
		}

		return *m_GameObjects[m_NameToEntity[_name]];
	}

	GameObject& Scene::GetGameObject(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Game Object with ID \"{0}\" does not exist!", id);
			static GameObject error;
			return error;
		}

		return *m_GameObjects[id];
	}

	Ref<GameObject> Scene::GetGameObjectByRef(const std::string& _name)
	{
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named \"{0}\" does not exist!", _name);
			return nullptr;
		}

		uint32_t id = m_NameToEntity[_name];
		return m_GameObjects[id];
	}

	Ref<GameObject> Scene::GetGameObjectByRef(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Game Object witd ID \"{0}\" does not exist!", id);
			return nullptr;
		}

		return m_GameObjects[id];
	}

	void Scene::DestroyGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named \"{0}\" does not exist in the scene!", _name);
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
			HD_CORE_ERROR("Destroy a game object that does not exist!");
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
			HD_CORE_ERROR("Get name of a game object that does not exist!");
			return "unknown";
		}

		return m_EntityToName[id];
	}

	void Scene::SetActiveCamera(GameObject& cam)
	{
		if (!cam.HasComponent<Camera>())
		{
			HD_CORE_ERROR("Set the object that is not a camera gameobject to be the camera object.");
			return;
		}
		m_ActiveCamera = &cam;
	}

	const GameObject& Scene::GetActiveCamera()
	{
		if (!m_ActiveCamera)
		{
			HD_CORE_ERROR("No active camera!");
			static GameObject cam;
			return cam;
		}
		return *m_ActiveCamera;
	}

}