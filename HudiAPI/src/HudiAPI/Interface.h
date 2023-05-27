#pragma once
#include "pch.h"

#include "HudiAPI.h"

#ifdef __cplusplus
//extern "C" {
#endif

#include <ECS.h>

	namespace Hudi {
		class ScriptEngine;
	}

	namespace hd_api {

		class ScriptComponent;
	
		class HD_API Component
		{
		public:
			virtual ~Component() = default;
		public:
			template <typename T>
			bool HasComponent() { return world->HasComponent<T>(m_Entity); }

			template <typename T>
			T& GetComponent() { return *world->GetComponent<T>(m_Entity); }

			template <typename T>
			bool Is() { return dynamic_cast<T*>(this); }

			void DestroyEntity() { world->DestroyEntity(m_Entity); }

			void SetActive(bool active) { world->SetActive(m_Entity, active); }
			bool IsActive() { return world->IsActive(m_Entity); }
		protected:
			ECS::Entity m_Entity = 0;
			ECS::World* world = nullptr;
		};

		class HD_API ScriptComponent : public Component
		{
		public:
			void Init(ECS::Entity entity, ECS::World* _world) { m_Entity = entity, world = _world; }

			virtual void Awake() {}
			virtual void Update(float dt) {}
		};

	}

#ifdef __cplusplus
//}
#endif