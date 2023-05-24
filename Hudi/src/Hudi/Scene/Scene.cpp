#include "hdpch.h"
#include "Scene.h"

#include "Components/Component.h"
#include "Components/CameraComponent.h"
#include "Components/SpriteRendererComponent.h"

#include "Hudi/Renderer/Renderer2D.h"

#include "ScriptEngine/ScriptEngine.h"

namespace Hudi {

	Scene::Scene(uint8_t index)
		: m_BuildIndex(index)
	{
		m_World = NewRef<ECS::World>();
		m_RenderSystem = m_World->RegisterSystem<RenderSystem>();
		m_Physics2DSystem = m_World->RegisterSystem<Physics2DSystem>();

		m_ScriptEngine = NewRef<ScriptEngine>(m_World.get());
	}

	Scene::~Scene()
	{
		m_ScriptEngine->DestroyEntities();
		if (!m_IsCopy)
			m_ScriptEngine->FreeLibraries();
		m_ScriptEngine = nullptr;

		for (auto& [id, object] : m_GameObjects)
		{
			object.Destroy();
		}
		m_World->Flush();

		m_GameObjects.clear();
		m_EntityToName.clear();
		m_NameToEntity.clear();

		m_RenderSystem = nullptr;
		m_Physics2DSystem = nullptr;
		m_World = nullptr;
	}

	void Scene::BeginScene()
	{
		HD_CORE_INFO("Start scene {0}.", m_BuildIndex);
		m_Physics2DSystem->Begin();
	}

	void Scene::EndScene()
	{
		HD_CORE_INFO("End scene {0}.", m_BuildIndex);
		m_Physics2DSystem->End();
	}

	void Scene::OnUpdate(float dt)
	{
		m_World->EachComponents<Component>([](Ref<Component> comp) { comp->Awake(); });
		m_ScriptEngine->AwakeScripts();
		
		m_World->EachComponents<Component>([dt](Ref<Component> comp) { comp->Update(dt); });
		m_ScriptEngine->UpdateScripts(dt);

		m_Physics2DSystem->OnUpdate(dt);
		m_RenderSystem->OnUpdate(dt, m_PrimaryCamera.GetEntityID());

		Invalidate();
	}

	void Scene::OnViewportResize(int width, int height)
	{
		if (width <= 0 || height <= 0)
			return;

		m_Width = width, m_Height = height;
		for (const auto& entt : m_World->View<Camera>())
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
			std::string name = m_EntityToName.at(id);

			m_GameObjects.erase(id);
			m_EntityToName.erase(id);
			m_NameToEntity.erase(name);

			go.Destroy();
		}
		m_World->Flush();
	}

	void Scene::Invalidate()
	{
		for (auto& [id, object] : m_GameObjects)
		{
			if (object.Exist())
				continue;
			DestroyGameObject(object);
			m_ScriptEngine->DestroyEntity(object.GetEntityID());
		}
		Flush();
	}

	Ref<Scene> Scene::Copy() const
	{
		Ref<Scene> newScene = NewRef<Scene>(this->m_BuildIndex);
		newScene->m_IsCopy = true;
		newScene->m_Width = this->m_Width;
		newScene->m_Height = this->m_Height;

		newScene->m_ScriptEngine->m_Libraries = this->m_ScriptEngine->m_Libraries;
		newScene->m_ScriptEngine->m_InstantiateFns = this->m_ScriptEngine->m_InstantiateFns;
		for (const auto& [id, object] : this->m_GameObjects)
		{
			const std::string& objName = this->m_EntityToName.at(id);
			// GameObject::Copy already copy the IDComponent
			GameObject newObject = newScene->CreateEmptyObjectWithUUID(object.GetUUID(), objName);
			newScene->RenameGameObject(objName, newObject);
			newObject.CopyComponents(object);

			auto& scripts = this->m_ScriptEngine->GetScripts(object.GetEntityID());
			for (auto& [scriptName, sc] : scripts)
			{
				newScene->m_ScriptEngine->AddScriptComponent(newObject.GetEntityID(), scriptName);
			}
		}
		const std::string& primaryCameraName = this->GetGameObjectName(this->m_PrimaryCamera);
		newScene->SetPrimaryCamera(primaryCameraName);

		return newScene;
	}

	GameObject Scene::CreateEmptyObject(const std::string& _name)
	{
		if (_name == "##unknown")
			return GameObject();
		GameObject obj = CreateEmptyObjectWithUUID(UUID(), _name);
		return obj;
	}

	GameObject Scene::CreateEmptyObjectWithUUID(UUID uuid, const std::string& _name)
	{
		if (_name == "##unknown")
			return GameObject();

		GameObject obj(m_World.get());
		obj.AddComponent<IDComponent>(uuid);

		uint32_t id = obj.GetEntityID();
		std::string name = FindValidName(_name);
		m_GameObjects[id] = obj;
		m_EntityToName[id] = name;
		m_NameToEntity[name] = id;
		return obj;
	}

	GameObject Scene::CreateGameObject(const std::string& _name)
	{
		if (_name == "##unknown")
			return GameObject();
		GameObject obj = CreateEmptyObject(_name);
		obj.AddComponent<Transform>();
		obj.AddComponent<SpriteRenderer>();
		return obj;
	}

	GameObject Scene::DuplicateObject(const std::string& _name)
	{
		if (_name == "##unknown")
			return GameObject();
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Cannot find object named '{0}' to duplicate!", _name);
			return GameObject();
		}

		GameObject src = GetGameObject(_name);
		std::string newObjectName = FindValidName(_name);
		GameObject newObject = CreateEmptyObject(newObjectName);
		newObject.CopyComponents(src);
		for (auto& [scriptName, sc] : m_ScriptEngine->GetScripts(src.GetEntityID()))
		{
			m_ScriptEngine->AddScriptComponent(newObject.GetEntityID(), scriptName);
		}
		return src;
	}

	GameObject Scene::DuplicateObject(const GameObject& src)
	{
		if (!HasGameObject(src))
		{
			HD_CORE_ERROR("Cannot find object to duplicate! (UUID: {0}", (uint64_t)src.GetUUID());
			return GameObject();
		}
		const std::string& srcName = GetGameObjectName(src);
		return DuplicateObject(srcName);
	}

	bool Scene::HasGameObject(const std::string& _name) const
	{
		if (_name == "##unknown")
			return false;
		return m_NameToEntity.find(_name) != m_NameToEntity.end();
	}

	bool Scene::HasGameObject(GameObject object) const
	{
		uint32_t id = object.GetEntityID();
		return m_GameObjects.find(id) != m_GameObjects.end() && m_GameObjects.at(id).world == m_World.get();
	}

	GameObject Scene::GetGameObject(const std::string& _name) const
	{
		if (_name == "##unknown")
			return GameObject();
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("Game Object named '{0}' does not exist!", _name);
			return GameObject();
		}

		return m_GameObjects.at(m_NameToEntity.at(_name));
	}

	GameObject Scene::GetGameObject(uint32_t id) const
	{
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Game Object with ID \"{0}\" does not exist!", id);
			return GameObject();
		}

		return m_GameObjects.at(id);
	}

	void Scene::DestroyGameObject(const std::string& _name)
	{
		if (_name == "##unknown")
			return;
		if (m_NameToEntity.find(_name) == m_NameToEntity.end())
		{
			HD_CORE_ERROR("GameObject named \"{0}\" does not exist in the scene!", _name);
			return;
		}

		GameObject object = m_GameObjects.at(m_NameToEntity.at(_name));
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

	std::string Scene::GetGameObjectName(GameObject object) const
	{
		uint32_t id = object.GetEntityID();
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Get the name of a GameObject that does not exist!");
			return "##unknown";
		}

		return m_EntityToName.at(id);
	}

	void Scene::RenameGameObject(const std::string& _name, GameObject object)
	{
		if (_name == "##unknown")
			return;
		uint32_t id = object.GetEntityID();
		if (m_EntityToName.find(id) == m_EntityToName.end())
		{
			HD_CORE_ERROR("Renamed GameObject with ID \"{0}\" that does not exist!", id);
			return;
		}

		std::string old_name = m_EntityToName.at(id);
		m_EntityToName[id] = _name;
		m_NameToEntity.erase(old_name);
		m_NameToEntity[_name] = id;
	}

	std::string Scene::FindValidName(const std::string& _name) const
	{
		std::string nonNum = _name;
		if (nonNum.back() == ')')
		{
			nonNum = nonNum.substr(0, nonNum.find_last_of('(') - 1);
		}

		int ind = 1;
		std::string name = nonNum;
		while (m_NameToEntity.find(name) != m_NameToEntity.end())
		{
			name = nonNum + " (" + std::to_string(ind++) + ")";
		}
		return name;
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

	bool Scene::SetPrimaryCamera(const std::string& cameraName)
	{
		if (cameraName == "##unknown")
			return false;
		GameObject camera = GetGameObject(cameraName);
		if (!camera.Valid())
			return false;
		return SetPrimaryCamera(camera);
	}

	void Scene::ResetPrimaryCamera()
	{
		m_PrimaryCamera.Reset();
	}

	GameObject Scene::GetPrimaryCamera() const
	{
		return m_PrimaryCamera;
	}

}