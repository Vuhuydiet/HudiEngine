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

	void ScriptEngine::FreeLibraries()
	{
		for (const auto& library : m_Libraries)
		{
			FreeLibrary(library);
		}
	}

	void ScriptEngine::AwakeScripts()
	{
		for (auto& [entity, scripts] : m_ObjectScripts)
		{
			for (auto& [scriptName, scriptComponent] : scripts)
			{
				scriptComponent->Awake();
			}
		}
	}

	void ScriptEngine::UpdateScripts(float dt)
	{
		for (auto& [entity, scripts] : m_ObjectScripts)
		{
			for (auto& [scriptName, scriptComponent] : scripts)
			{
				scriptComponent->Update(dt);
			}
		}
	}

	void ScriptEngine::LoadScript(std::filesystem::path libpath)
	{
		HMODULE library = LoadLibrary((wchar_t*)libpath.c_str());
		if (!library)
		{
			HD_CORE_ERROR("Cannot load '{0}'!", libpath.string());
			return;
		}
		m_Libraries.push_back(library);

		int count = 1;
		while (true)
		{
			std::string func_name = "InstantiateScript" + std::to_string(count++);
			InstantiateScriptComponentFunc func = reinterpret_cast<InstantiateScriptComponentFunc>(GetProcAddress(library, func_name.c_str()));
			if (!func)
				break;
			m_InstantiateFns[libpath.filename().stem().string()].push_back(func);
		}
	}

	void ScriptEngine::AssimilateInput() const
	{
		for (auto& library : m_Libraries)
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

	void ScriptEngine::AddScriptComponent(ECS::Entity entity, const std::string& script)
	{
		if (!world->Exists(entity))
		{
			HD_CORE_ERROR("Entity {0} does not exists in world!", entity);
			return;
		}
		if (m_InstantiateFns.find(script) == m_InstantiateFns.end())
		{
			HD_CORE_ERROR("Cannot find '{0}' script!", script);
			return;
		}
		if (m_ObjectScripts.find(entity) != m_ObjectScripts.end())
		{
			for (auto& [scriptName, sc] : m_ObjectScripts.at(entity))
			{
				if (scriptName == script)
					return;
			}
		}

		for (auto& func : m_InstantiateFns.at(script))
		{
			hd_api::Behaviour* sc = func();
			sc->Init(entity, world);
			m_ObjectScripts[entity].push_back({ script, sc });
		}
		
	}
	
	void ScriptEngine::RemoveScriptComponent(ECS::Entity entity, const std::string& script)
	{
		if (!world->Exists(entity))
		{
			HD_CORE_ERROR("Entity {0} does not exists in world!", entity);
			return;
		}
		if (m_InstantiateFns.find(script) == m_InstantiateFns.end())
		{
			HD_CORE_ERROR("Cannot find '{0} script!", script);
			return;
		}

		for (auto& p : m_ObjectScripts.at(entity))
		{
			auto& [name, comp] = p;
			if (name != script)
				continue;

			delete comp;
			std::swap(p, m_ObjectScripts[entity].back());
			m_ObjectScripts[entity].pop_back();
			break;
		}
	}

	void ScriptEngine::DestroyEntity(ECS::Entity entity)
	{
		if (m_ObjectScripts.find(entity) == m_ObjectScripts.end())
		{
			HD_CORE_ERROR("Entity {0} does not have any script component!", entity);
			return;
		}
		for (auto& [scriptName, sc] : m_ObjectScripts.at(entity))
		{
			delete sc;
		}
		m_ObjectScripts.erase(entity);
	}

	void ScriptEngine::DestroyEntities()
	{
		while (!m_ObjectScripts.empty())
		{
			auto it = m_ObjectScripts.begin();
			DestroyEntity(it->first);
		}
	}

	std::vector<std::pair<std::string, hd_api::Behaviour*>> ScriptEngine::GetScripts(ECS::Entity entity)
	{
		if (m_ObjectScripts.find(entity) == m_ObjectScripts.end())
		{
			return std::vector<std::pair<std::string, hd_api::Behaviour*>>();
		}
		return m_ObjectScripts.at(entity);
	}

}