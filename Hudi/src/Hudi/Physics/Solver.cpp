#include "hdpch.h"
#include "Solver.h"

namespace Hudi {

	void Solver::Solve(
		ECS::Entity entt_1, Vec3& ds_1,
		ECS::Entity entt_2, Vec3& ds_2)
	{
		auto& rigid_1 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_1);
		auto& collider_1 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_1);

		auto& rigid_2 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_2);
		auto& collider_2 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_2);

		if (rigid_1.IsStatic())
		{
			if (rigid_2.IsDynamic()) 
			{
				Solver::DynamicStatic(
					entt_2, ds_2,
					entt_1, ds_1
				);
			}
		}
		else  if (rigid_1.IsDynamic()) 
		{
			if (rigid_2.IsStatic())
			{
				Solver::DynamicStatic(
					entt_1, ds_1,
					entt_2, ds_2
				);
			}
			else if (rigid_2.IsDynamic())
			{
				Solver::DynamicDynamic(
					entt_1, ds_1,
					entt_2, ds_2
				);
			}
		}
	}

	void Solver::DynamicStatic(
		ECS::Entity dyn_entt, Vec3& ds_dyn,
		ECS::Entity sta_entt, Vec3& ds_2)
	{
		const auto& dyn_collider = ECS::Coordinator::GetComponent<BoxCollider2D>(dyn_entt);
		auto& dyn_rigid = ECS::Coordinator::GetComponent<RigidBody2D>(dyn_entt);

		const auto& sta_collider = ECS::Coordinator::GetComponent<BoxCollider2D>(sta_entt);

		if (dyn_collider.maxPoint.x < sta_collider.minPoint.x)
		{
			//1
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y)
			{
			}
			//7
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y)
			{
			}
			//4
			else 
			{
				ds_dyn.x = sta_collider.minPoint.x - dyn_collider.maxPoint.x - 1;
				dyn_rigid.velocity.x = 0.f;
			}
		}
		else if (dyn_collider.minPoint.x > sta_collider.maxPoint.x) 
		{
			//3
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y) 
			{
			}
			//9
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y)
			{
			}
			//6
			else 
			{
				ds_dyn.x = sta_collider.maxPoint.x - dyn_collider.minPoint.x + 1;
				dyn_rigid.velocity.x = 0.f;
			}
		}
		else
		{
			//2
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y) 
			{
				ds_dyn.y = sta_collider.minPoint.y - dyn_collider.maxPoint.y - 1;
				dyn_rigid.velocity.y = 0.f;
			}
			//8
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y) 
			{
				ds_dyn.y = sta_collider.maxPoint.y - dyn_collider.minPoint.y + 1;
				dyn_rigid.velocity.y = 0.f;
			}
		}
	}

	void Solver::DynamicDynamic(
		ECS::Entity entt_1, Vec3& ds_1,
		ECS::Entity entt_2, Vec3& ds_2)
	{
		const auto& collider_1 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_1);
		auto& rigid_1 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_1);

		const auto& collider_2 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_2);
		auto& rigid_2 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_2);

		if (collider_1.maxPoint.x < collider_2.minPoint.x)
		{
			//1
			if (collider_1.maxPoint.y < collider_2.minPoint.y)
			{
			}
			//7
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
			}
			//4
			else 
			{
				ds_1.x = 0;
				ds_2.x = 0;
			}
		}
		else if (collider_1.minPoint.x > collider_2.maxPoint.x) 
		{
			//3
			if (collider_1.maxPoint.y < collider_2.minPoint.y)
			{
			}
			//9
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
			}
			//6
			else 
			{
				ds_1.x = 0;
				ds_2.x = 0;
			}
		}
		else
		{
			//2
			if (collider_1.maxPoint.y < collider_2.minPoint.y) 
			{
				ds_1.y = collider_2.minPoint.y - collider_1.maxPoint.y - 1;
				rigid_1.velocity.y = 0.f;
			}
			//8
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
				ds_2.y = collider_1.minPoint.y - collider_2.maxPoint.y - 1;
				rigid_2.velocity.y = 0.f;
			}
		}
	}
}