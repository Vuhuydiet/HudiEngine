#include "hdpch.h"
#include "ScriptEngine.h"

#include "Hudi/Core/Input.h"

#include <Windows.h>

namespace Hudi {

	ScriptEngine::ScriptEngine(ECS::World* _world)
		: world(_world)
	{
	}

	ScriptEngine::~ScriptEngine()
	{
		
	}

	void ScriptEngine::CopyLibraries(const ScriptEngine& src)
	{
		this->m_Libraries = src.m_Libraries;
		this->m_LibraryBehaviours = src.m_LibraryBehaviours;
		this->m_CreationFns = src.m_CreationFns;
	}

	void ScriptEngine::FreeLibraries()
	{
		for (const auto& [name, library] : m_Libraries)
		{
			FreeLibrary(library);
		}
	}

	void ScriptEngine::AwakeBehaviours()
	{
		for (auto& [entity, scripts] : m_ObjectScripts)
		{
			for (auto& [scriptName, scriptComponent] : scripts)
			{
				scriptComponent->Awake();
			}
		}
	}

	void ScriptEngine::UpdateBehaviours(float dt)
	{
		for (auto& [entity, scripts] : m_ObjectScripts)
		{
			for (auto& [scriptName, scriptComponent] : scripts)
			{
				scriptComponent->Update(dt);
			}
		}
	}

	void ScriptEngine::LoadScriptLibrary(std::filesystem::path libpath)
	{
		std::string libname = libpath.stem().string();
		if (m_Libraries.find(libname) != m_Libraries.end())
			return;

		HMODULE library = LoadLibrary((wchar_t*)libpath.c_str());
		if (!library)
		{
			HD_CORE_ERROR("Cannot load '{0}'!", libpath.string());
			return;
		}
		m_Libraries[libname] = library;

		GetBehaviourComponentNamesFunc GetNamesFn = reinterpret_cast<GetBehaviourComponentNamesFunc>(GetProcAddress(library, "GetBehaviourComponentNames"));
		if (!GetNamesFn)
		{
			HD_CORE_ERROR("Cannot find function 'GetBehaviourComponentNames' in library {0}!", libname);
			return;
		}
		const char** buffers = (const char**)malloc(MAX_BEHAVIOUR_COMP * sizeof(const char*));
		if (!buffers)
		{
			HD_CORE_ERROR("Cannot allocate memory for 'buffers'!");
			return;
		}
		memset(buffers, 0, MAX_BEHAVIOUR_COMP);
		size_t size = 0;
		GetNamesFn(buffers, &size);
		for (int i = 0; i < std::min(size, MAX_BEHAVIOUR_COMP); i++)
		{
			std::string behav_name = buffers[i];
			std::string ins_func_name = "Instantiate" + behav_name;
			std::string des_func_name = "Destroy" + behav_name;
			InstantiateScriptComponentFunc instantiate = reinterpret_cast<InstantiateScriptComponentFunc>(GetProcAddress(library, ins_func_name.c_str()));
			DestroyScriptComponentFunc destroy = reinterpret_cast<DestroyScriptComponentFunc>(GetProcAddress(library, des_func_name.c_str()));
			if (!instantiate|| !destroy)
				continue;
			m_LibraryBehaviours[libname].push_back(behav_name);
			m_CreationFns[behav_name] = { instantiate, destroy };
		}
		free(buffers);
	}

	void ScriptEngine::AssimilateInput() const
	{
		for (auto& [name, library] : m_Libraries)
		{
			AssimilateInputFunc func = reinterpret_cast<AssimilateInputFunc>(GetProcAddress(library, "SetInputSource"));
			if (!func)
			{
				HD_CORE_ERROR("Cannot find 'SetInputSource' function!");
				continue;
			}
			func(EventManager::GetInstance());
		}
	}

	void ScriptEngine::AddBehaviourComponent(GameObject object, const std::string& script)
	{
		ECS::Entity entity = object.GetEntityID();
		if (!world->Exists(entity))
		{
			HD_CORE_ERROR("Entity {0} does not exists in world!", entity);
			return;
		}
		if (m_CreationFns.find(script) == m_CreationFns.end())
		{
			HD_CORE_ERROR("Cannot find '{0}' component!", script);
			return;
		}

		hd_api::Behaviour* bc = m_CreationFns.at(script).Instantiate(object);
		bc->Init(entity, world);
		m_ObjectScripts[entity].insert({ script, bc });
	}
	
	void ScriptEngine::RemoveBehaviourComponent(GameObject object, const std::string& script)
	{
		ECS::Entity entity = object.GetEntityID();
		if (!world->Exists(entity))
		{
			HD_CORE_ERROR("Entity {0} does not exists in world!", entity);
			return;
		}
		if (m_CreationFns.find(script) == m_CreationFns.end())
		{
			HD_CORE_ERROR("Cannot find '{0} script!", script);
			return;
		}

		m_CreationFns.at(script).Destroy(object);
		m_ObjectScripts[entity].erase({ script, nullptr });
	}

	void ScriptEngine::DestroyEntity(ECS::Entity entity)
	{
		/*if (m_ObjectScripts.find(entity) == m_ObjectScripts.end())
		{
			HD_CORE_ERROR("Entity {0} does not have any script component!", entity);
			return;
		}*/
		/*for (auto& [scriptName, sc] : m_ObjectScripts.at(entity))
		{
			delete sc;
			sc = nullptr;
		}*/
		m_ObjectScripts.erase(entity);
	}

	void ScriptEngine::DestroyEntities()
	{
		/*for (auto& [entity, comp_infos] : m_ObjectScripts)
		{
			for (auto& [name, comp] : comp_infos)
			{
				delete comp;
				comp = nullptr;
			}
		}*/
		m_ObjectScripts.clear();
	}

	std::vector<std::string> ScriptEngine::GetLibraryBehaviours(const std::string& libname)
	{
		if (m_LibraryBehaviours.find(libname) == m_LibraryBehaviours.end())
		{
			HD_CORE_ERROR("Cannot find library '{0}'!", libname);
			return std::vector<std::string>();
		}
		return m_LibraryBehaviours.at(libname);
	}

	const std::unordered_set<ComponentInfo>& ScriptEngine::GetBehaviours(ECS::Entity entity)
	{
		return m_ObjectScripts[entity];
	}

}