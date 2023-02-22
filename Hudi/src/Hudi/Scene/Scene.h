#pragma once
#include "hdpch.h"

#include "GameObject.h"

namespace Hudi {

	class Scene
	{
	public:
		Scene();

		void BeginScene();
		void EndScene();

		GameObject& CreateGameObject(std::string _name);
		void DestroyGameObject(const std::string& _name);

		void SetLoadFunction(std::function<void(Scene&)> fn) { m_LoadFn = fn; }

	public:
		uint8_t buildIndex;
	private:
		std::function<void(Scene&)> m_LoadFn = nullptr;
		std::unordered_map<std::string, Ref<GameObject>> m_GameObjects;
	};

}