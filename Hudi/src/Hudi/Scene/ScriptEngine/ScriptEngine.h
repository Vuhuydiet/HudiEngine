#pragma once
#include <HudiAPI/Interface.h>
#include "Hudi/Events/Event.h"
#include "Hudi/Scene/GameObject.h"

#include <utility>
#include <filesystem>

namespace Hudi {

	struct ComponentInfo
	{
		std::string name;
		hd_api::Behaviour* data = nullptr;
		bool operator== (const ComponentInfo& other) const { return this->name == other.name; }
	};
}

namespace std {

	template <>
	struct hash<Hudi::ComponentInfo>
	{
		std::size_t operator()(const Hudi::ComponentInfo& component) const
		{
			return hash<std::string>()(component.name);
		}
	};

}

namespace Hudi {

	class ScriptEngine
	{
	private:
		using AssimilateInputFunc = void (*)(EventManager*);
		using GetBehaviourComponentNamesFunc = void (*)(const char**, size_t*);
		using InstantiateScriptComponentFunc = hd_api::Behaviour* (*)(GameObject);
		using DestroyScriptComponentFunc = void (*)(GameObject);

		const size_t MAX_BEHAVIOUR_COMP = 20;
	public:
		ScriptEngine() = default;
		ScriptEngine(ECS::World* _world);
		~ScriptEngine();
		void CopyLibraries(const ScriptEngine& src);
		// Really need to do before deleting the ScriptEngine
		// Must free after deleted all the components created by the libraries
		void FreeLibraries();

		void AssimilateInput() const;

		void LoadScriptLibrary(std::filesystem::path libpath);

		void AddBehaviourComponent(GameObject object, const std::string& script);
		void RemoveBehaviourComponent(GameObject object, const std::string& script);
		
		const std::map<std::string, std::vector<std::string>>& GetLibraries() const { return m_LibraryBehaviours; }
		std::vector<std::string> GetLibraryBehaviours(const std::string& libname);
		const std::unordered_set<ComponentInfo>& GetBehaviours(ECS::Entity entity);

		void AwakeBehaviours();
		void UpdateBehaviours(float dt);

		void DestroyEntity(ECS::Entity entity);
		// Don't really need to do before deleting the ScriptEngine
		void DestroyEntities();

	private:
		std::map<std::string, HMODULE> m_Libraries;
		std::map<std::string, std::vector<std::string>> m_LibraryBehaviours;

		struct CreationFunctions 
		{
			InstantiateScriptComponentFunc Instantiate;
			DestroyScriptComponentFunc Destroy;
		};
		std::map<std::string, CreationFunctions> m_CreationFns;

		std::unordered_map<ECS::Entity, std::unordered_set<ComponentInfo>> m_ObjectScripts;
	private:
		ECS::World* world = nullptr;
	};

}