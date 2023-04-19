#include "hdpch.h"
#include "CollisionDetector.h"

#include "Solver.h"

#include "Hudi/Scene/Components/BehaviourComponent.h"

namespace Hudi {
#ifdef PHYSICS_ENGINE
	void CollisionDetector::Update(std::unordered_map<ECS::Entity, Vec3>& Entities_ds)
	{
		Solve(Entities_ds);

		for (auto& [entt, ds] : Entities_ds)
		{
			ECS::Coordinator::GetComponent<Transform>(entt).Translate(ds);
		}
	}

	void CollisionDetector::Solve(std::unordered_map<ECS::Entity, Vec3>& Entities_ds)
	{
		for (auto& [entt_1, ds_1] : Entities_ds) 
		{
			for (auto& [entt_2, ds_2] : Entities_ds) 
			{
				if (entt_1 == entt_2)
					break;

				auto& bd_1 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_1);
				auto& bd_2 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_2);
				if (bd_1.IsStatic() && bd_2.IsStatic())
					continue;

				auto& box_1 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_1);
				auto& box_2 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_2);

				box_1.Translate(ds_1);
				box_2.Translate(ds_2);
				bool hasCollision = box_1.TestCollision(&box_2);
				box_1.Translate(Vec3::Zero() - ds_1);
				box_2.Translate(Vec3::Zero() - ds_2);

				if (hasCollision)
				{
					if (bd_1.IsDynamic())
						CallOnCollisionEnter2D(entt_1, entt_2);
					if (bd_2.IsDynamic())
						CallOnCollisionEnter2D(entt_2, entt_1);

					Solver::Solve(entt_1, ds_1, entt_2, ds_2);
				}
			}
		}
	}

	void CollisionDetector::CallOnCollisionEnter2D(ECS::Entity entt_1, ECS::Entity entt_2)
	{
		for (auto& component : ECS::Coordinator::GetComponents(entt_1))
		{
			if (component->Is<MonoBehaviour>())
				std::static_pointer_cast<MonoBehaviour>(component)->OnCollisionEnter2D(Collision2D(entt_1, entt_2));
		}
	}
#endif 

}