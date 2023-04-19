#pragma once
#include "hdpch.h"

#include <ECS.h>

#include "Hudi/Scene/Components/TransformComponent.h"
#include "Hudi/Scene/Components/RigidBody2DComponent.h"
#include "Hudi/Scene/Components/Collider2DComponent.h"

#include "Collision2D.h"

namespace Hudi {

#ifdef PHYSICS_ENGINE
	class CollisionDetector
	{
	public:

		void Update(std::unordered_map<ECS::Entity, Vec3>& Entities_ds);
		void Solve(std::unordered_map<ECS::Entity, Vec3>& Entities_ds);
		
		void CallOnCollisionEnter2D(ECS::Entity entt_1, ECS::Entity entt_2);
	};
#endif

}