#include "hdpch.h"
#include "Scene.h"

#include "Components/Component.h"

#include "Hudi/Renderer/Renderer2D.h"

namespace Hudi {

	Scene::Scene(uint8_t index)
		: m_BuildIndex(index)
	{
		m_World = NewRef<ECS::World>();
	}

	Scene::~Scene()
	{
		m_RenderSystem = nullptr;

		for (auto& [id, object] : m_GameObjects)
		{
			object.Destroy();
		}
		m_World->Flush();

		m_GameObjects.clear();
		m_EntityToName.clear();
		m_NameToEntity.clear();

		m_World = nullptr;
	}

	void Scene::BeginScene()
	{
		HD_CORE_INFO("Start scene {0}", m_BuildIndex);
		m_RenderSystem = m_World->RegisterSystem<RenderSystem>();
		m_RenderSystem->scene = this;
	}

	void Scene::EndScene()
	{
		HD_CORE_INFO("End scene {0}", m_BuildIndex);
	}

	void Scene::OnUpdate(float dt)
	{
		m_World->EachComponents<Component>([](Ref<Component> comp) { comp->Awake(); });
		m_World->EachComponents<Component>([dt](Ref<Component> comp) { comp->Update(dt); });
		
		m_RenderSystem->OnUpdate(dt);

		m_World->Flush();
	}

	void Scene::OnViewportResize(int width, int height)
	{
		if (width <= 0 || height <= 0)
			return;

		m_Width = width, m_Height = height;
		for (auto& entt : m_World->View<Camera>())
		{
			auto& camera = m_World->GetComponent<Camera>(entt);
			camera->Resize((float)width, (float)height);
		}
	}

	void Scene::Flush()
	{
		while (!m_DestroyedObjects.empty())
		{
			uint32_t id = m_DestroyedObjects.front();
			m_DestroyedObjects.pop();

			GameObject go = m_GameObjects.at(id);
			const std::string& name = m_EntityToName.at(id);

			m_GameObjects.erase(id);
			m_EntityToName.erase(id);
			m_NameToEntity.erase(name.c_str());

			go.Destroy();
		}
		m_World->Flush();
	}

	GameObject Scene::CreateEmptyObject(const std::string& _name)
	{
		std::string name = _name;

		static uint8_t ind = 1;
		if (m_NameToEntity.find(name.c_str()) != m_NameToEntity.end())
			name += " (" + std::to_string(ind++) + ")";

		GameObject obj(m_World.get());

		uint32_t id = obj.GetEntityID();
		m_GameObjects[id] = obj;
		m_EntityToName[id] = name;
		m_NameToEntity[name.c_str()] = id;
		return obj;
	}

	GameObject Scene::CreateGameObject(const std::string& _name)
	{
		GameObject obj = CreateEmptyObject(_name);
		obj.AddComponent<Transform>();
		obj.AddComponent<SpriteRenderer>();
		return obj;
	}

	bool Scene::HasGameObject(const std::string& _name)
	{
		return m_NameToEntity.find(_name.c_str()) != m_NameToEntity.end();
	}

	bool Scene::HasGameObject(GameObject object)
	{
		uint32_t id = object.GetEntityID();
		return m_GameObjects.find(id) != m_GameObjects.end() && m_GameObjects.at(id).world == m_World.get();
	}

	GameObject Scene::GetGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name.c_str()) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named \"{0}\" does not exist!", _name);
			return nullptr;
		}

		return m_GameObjects.at(m_NameToEntity.at(_name.c_str()));
	}

	GameObject Scene::GetGameObject(uint32_t id)
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Game Object with ID \"{0}\" does not exist!", id);
			return nullptr;
		}

		return m_GameObjects.at(id);
	}

	void Scene::DestroyGameObject(const std::string& _name)
	{
		if (m_NameToEntity.find(_name.c_str()) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("GameObject named \"{0}\" does not exist in the scene!", _name);
			return;
		}

		GameObject object = m_GameObjects.at(m_NameToEntity.at(_name.c_str()));
		DestroyGameObject(object);
	}

	void Scene::DestroyGameObject(GameObject object)
	{
		uint32_t id = object.GetEntityID();
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Destroyed a GameObject that does not exist!");
			return;
		}

		m_DestroyedObjects.push(id);
	}

	std::string Scene::GetGameObjectName(GameObject object)
	{
		uint32_t id = object.GetEntityID();
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Get the name of a GameObject that does not exist!");
			return "unknown";
		}

		return m_EntityToName.at(id);
	}

	void Scene::RenameGameObject(const std::string& _name, GameObject object)
	{
		uint32_t id = object.GetEntityID();
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Renamed GameObject with ID \"{0}\" that does not exist!", id);
			return;
		}

		std::string old_name = m_EntityToName.at(id);
		m_EntityToName[id] = _name;
		m_NameToEntity.erase(old_name.c_str());
		m_NameToEntity[_name.c_str()] = id;
	}

	bool Scene::SetPrimaryCamera(GameObject camera)
	{
		if (!HasGameObject(camera) || !camera.HasComponent<Transform>() || !camera.HasComponent<Camera>())
		{
			HD_CORE_ERROR("Setting the primary camera object to be an object that does not exists in the scene!");
			return false;
		}
		m_PrimaryCamera = camera;
		return true;
	}

	void Scene::ResetPrimaryCamera()
	{
		m_PrimaryCamera.Reset();
	}

	GameObject Scene::GetPrimaryCamera()
	{
		return m_PrimaryCamera;
	}

}