#pragma once

#include <ECS.h>

#include "Hudi/Physics/Collision2D.h"
#include "Collider2DComponent.h"

namespace Hudi {

	struct Behaviour : public ECS::Component
	{
		virtual ~Behaviour() = default;
		virtual const char* ToString() const override { return "Behaviour"; }

		void DestroyEntity() { ECS::Coordinator::DestroyEntity(m_Entt); }

		void SetActive(bool active) { ECS::Coordinator::SetActive(m_Entt, active); }
		bool IsActive() { return ECS::Coordinator::IsActive(m_Entt); }

	};


	// Derived from Behaviour
	class MonoBehaviour : public Behaviour
	{
	public:
		virtual ~MonoBehaviour() = default;
		virtual const char* ToString() const override { return "MonoBehaviour"; }

		bool IsAwakable() const override { return true; }
		bool IsUpdatable() const override { return true; }

		virtual void OnCollisionEnter2D(Collision2D collision) {}
	};

}