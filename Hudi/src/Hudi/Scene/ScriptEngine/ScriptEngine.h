#pragma once
#include <HudiAPI/Interface.h>

#include <utility>
#include <filesystem>

namespace Hudi {

	class ScriptEngine
	{
	private:
		using InstantiateScriptComponentFunc = hd_api::ScriptComponent* (*)();
	public:
		ScriptEngine() = default;
		ScriptEngine(ECS::World* _world);
		~ScriptEngine();
		void FreeLibraries();

		void AwakeScripts();
		void UpdateScripts(float dt);

		void LoadScript(std::filesystem::path libpath);
		void AddScriptComponent(ECS::Entity entity, const std::string& script);
		void RemoveScriptComponent(ECS::Entity entity, const std::string& script);

		void DestroyEntity(ECS::Entity entity);
		void DestroyEntities();

		std::vector<std::pair<std::string, hd_api::ScriptComponent*>> GetScripts(ECS::Entity entity);
	private:
		std::vector<HMODULE> m_Libraries;
		std::map<std::string, InstantiateScriptComponentFunc> m_InstantiateFns;

		std::unordered_map<ECS::Entity, std::vector<std::pair<std::string, hd_api::ScriptComponent*>>> m_ObjectScripts;
	private:
		ECS::World* world = nullptr;
		friend class Scene;
	};

}