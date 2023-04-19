#pragma once
#include "hdpch.h"

#include <ECS.h>

namespace Hudi {

#ifdef PHYSICS_ENGINE
	struct Collision2D
	{
		Collision2D(ECS::Entity _entt, ECS::Entity _otherEntt)
			: entity(_entt), otherEntity(_otherEntt) {}

		template <typename T>
		T GetEntityComponent() const { return ECS::Coordinator::GetComponent<T>(entity); }

		template <typename T>
		T GetOtherEntityComponent() const { return ECS::Coordinator::GetComponent<T>(otherEntity); }

	private:
		ECS::Entity entity, otherEntity;
	};
#endif

}