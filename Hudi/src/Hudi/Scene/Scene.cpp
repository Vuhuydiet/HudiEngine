#include "hdpch.h"
#include "Scene.h"

#include "CameraComponent.h"

namespace Hudi {

	Scene::Scene()
	{
		static int index = 0;
		buildIndex = index++;
	}

	void Scene::BeginScene()
	{
		HD_CORE_INFO("Start scene {0}", buildIndex);

		GameObject& mainCamera = CreateGameObject("Main Camera");
		mainCamera.AddComponent<Camera>();
		m_ActiveCamera = &mainCamera.GetComponent<Camera>();

		if (m_LoadFn)
			m_LoadFn(*this);
	}

	void Scene::EndScene()
	{
		HD_CORE_INFO("End scene {0}", buildIndex);
		m_GameObjects.clear();
	}

	GameObject& Scene::CreateGameObject(std::string _name)
	{
		static uint8_t ind = 1;
		if (m_GameObjects.find(_name) != m_GameObjects.end())
			_name += std::to_string(ind++);
		
		m_GameObjects[_name] = NewRef<GameObject>();
		return *m_GameObjects[_name];
	}

	void Scene::DestroyGameObject(const std::string& _name)
	{
		m_GameObjects.erase(_name);
	}

}