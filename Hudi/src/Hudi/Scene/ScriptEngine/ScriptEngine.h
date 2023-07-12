#pragma once
#include <HudiAPI/Interface.h>
#include "Hudi/Events/Event.h"

#include <utility>
#include <filesystem>

namespace Hudi {

	class ScriptEngine
	{
	private:
		using InstantiateScriptComponentFunc = hd_api::Behaviour* (*)();
		using AssimilateInputFunc = void (*)(EventManager*);
	public:
		ScriptEngine() = default;
		ScriptEngine(ECS::World* _world);
		~ScriptEngine();
		void FreeLibraries();

		void AssimilateInput() const;

		void LoadScript(std::filesystem::path libpath);
		void AddScriptComponent(ECS::Entity entity, const std::string& script);
		void RemoveScriptComponent(ECS::Entity entity, const std::string& script);

		void AwakeScripts();
		void UpdateScripts(float dt);

		void DestroyEntity(ECS::Entity entity);
		void DestroyEntities();

		std::vector<std::pair<std::string, hd_api::Behaviour*>> GetScripts(ECS::Entity entity);
	private:
		std::vector<HMODULE> m_Libraries;
		std::map<std::string, std::vector<InstantiateScriptComponentFunc>> m_InstantiateFns;

		std::unordered_map<ECS::Entity, std::vector<std::pair<std::string, hd_api::Behaviour*>>> m_ObjectScripts;
	private:
		ECS::World* world = nullptr;
		friend class Scene;
	};

}