#pragma once
#include "hdpch.h"

#include <ECS.h>

#include "Hudi/Components/TransformComponent.h"
#include "Hudi/Components/RigidBody2DComponent.h"
#include "Hudi/Components/Collider2DComponent.h"

#include "Hudi/Physics/Collision2D.h"

namespace Hudi {

	class Solver 
	{
	public:
		static void Solve(
			ECS::Entity entt_1, Vec3& ds_1,
			ECS::Entity entt_2, Vec3& ds_2
		);

	private:
		static void DynamicStatic(
			ECS::Entity dyn_entt, Vec3& ds_dyn,
			ECS::Entity sta_entt, Vec3& ds_sta
		);
		static void DynamicDynamic(
			ECS::Entity entt_1, Vec3& ds_1,
			ECS::Entity entt_2, Vec3& ds_2
		);
	};
}