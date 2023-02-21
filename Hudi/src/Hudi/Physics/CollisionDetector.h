#pragma once
#include "hdpch.h"

#include <ECS.h>

#include "Hudi/Components/TransformComponent.h"
#include "Hudi/Components/RigidBody2DComponent.h"
#include "Hudi/Components/Collider2DComponent.h"

#include "Collision2D.h"

namespace Hudi {

	class CollisionDetector
	{
	public:

		void Update(std::unordered_map<ECS::Entity, Vec3>& Entities_ds);
		void Solve(std::unordered_map<ECS::Entity, Vec3>& Entities_ds);
		
		void CallOnCollisionEnter2D(ECS::Entity entt_1, ECS::Entity entt_2);
	};

}