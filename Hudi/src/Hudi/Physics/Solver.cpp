#include "hdpch.h"
#include "Solver.h"

namespace Hudi {

#ifdef PHYSICS_ENGINE
	void Solver::Solve(
		ECS::Entity entt_1, Vec3& ds_1,
		ECS::Entity entt_2, Vec3& ds_2)
	{
		auto& body_1 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_1);
		auto& collider_1 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_1);

		auto& body_2 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_2);
		auto& collider_2 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_2);

		if (body_1.IsStatic())
		{
			if (body_2.IsDynamic()) 
			{
				Solver::DynamicStatic(
					entt_2, ds_2,
					entt_1, ds_1
				);
			}
		}
		else  if (body_1.IsDynamic()) 
		{
			if (body_2.IsStatic())
			{
				Solver::DynamicStatic(
					entt_1, ds_1,
					entt_2, ds_2
				);
			}
			else if (body_2.IsDynamic())
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
		auto& dyn_body = ECS::Coordinator::GetComponent<RigidBody2D>(dyn_entt);

		const auto& sta_collider = ECS::Coordinator::GetComponent<BoxCollider2D>(sta_entt);

		if (dyn_collider.maxPoint.x < sta_collider.minPoint.x)
		{
			//1
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y)
			{
				Vec3 A = dyn_collider.maxPoint;
				Vec3 B = sta_collider.minPoint;

				DynamicStaticCorner(A, B, ds_dyn);
			}
			//7
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y)
			{
				Vec3 A = { dyn_collider.maxPoint.x, dyn_collider.minPoint.y };
				Vec3 B = { sta_collider.minPoint.x, sta_collider.maxPoint.y };
				
				DynamicStaticCorner(A, B, ds_dyn);
			}
			//4
			else 
			{
				ds_dyn.x = sta_collider.minPoint.x - dyn_collider.maxPoint.x - 1;
				dyn_body.velocity.x = 0.f;
			}
		}
		else if (dyn_collider.minPoint.x > sta_collider.maxPoint.x) 
		{
			//3
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y) 
			{
				Vec3 A = { dyn_collider.minPoint.x, dyn_collider.maxPoint.y };
				Vec3 B = { sta_collider.maxPoint.x, sta_collider.minPoint.y };

				DynamicStaticCorner(A, B, ds_dyn);
			}
			//9
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y)
			{
				Vec3 A = dyn_collider.minPoint;
				Vec3 B = sta_collider.maxPoint;

				DynamicStaticCorner(A, B, ds_dyn);
			}
			//6
			else 
			{
				ds_dyn.x = sta_collider.maxPoint.x - dyn_collider.minPoint.x + 1;
				dyn_body.velocity.x = 0.f;
			}
		}
		else
		{
			//2
			if (dyn_collider.maxPoint.y < sta_collider.minPoint.y) 
			{
				ds_dyn.y = sta_collider.minPoint.y - dyn_collider.maxPoint.y - 1;
				dyn_body.velocity.y = 0.f;
			}
			//8
			else if (dyn_collider.minPoint.y > sta_collider.maxPoint.y) 
			{
				ds_dyn.y = sta_collider.maxPoint.y - dyn_collider.minPoint.y + 1;
				dyn_body.velocity.y = 0.f;
			}
		}
	}

	void Solver:: DynamicStaticCorner(Vec3 A, Vec3 B, Vec3& ds_dyn)
	{
		float tanDS = abs(ds_dyn.x / ds_dyn.y);
		float tanAB = abs((A.x - B.x) / (A.y - B.y));

		if (tanDS > tanAB)
		{
			ds_dyn.y = 0;
		}
		else if (tanDS < tanAB)
		{
			ds_dyn.y = 0;
		}
		else
		{
			ds_dyn = Vec3(0);
		}
	}

	void Solver::DynamicDynamic(
		ECS::Entity entt_1, Vec3& ds_1,
		ECS::Entity entt_2, Vec3& ds_2)
	{
		auto& collider_1 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_1);
		auto& body_1 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_1);

		auto& collider_2 = ECS::Coordinator::GetComponent<BoxCollider2D>(entt_2);
		auto& body_2 = ECS::Coordinator::GetComponent<RigidBody2D>(entt_2);

		if (collider_1.maxPoint.x < collider_2.minPoint.x)
		{
			//1
			if (collider_1.maxPoint.y < collider_2.minPoint.y)
			{
				ECS::Coordinator::GetComponent<Transform>(entt_2).Translate(ds_2);
				ds_2 = Vec3(0);

				Vec3 A = collider_1.maxPoint;
				Vec3 B = collider_2.minPoint;

				DynamicStaticCorner(A, B, ds_1);
			}
			//7
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
				ECS::Coordinator::GetComponent<Transform>(entt_1).Translate(ds_1);
				ds_1 = Vec3(0);

				Vec3 A = { collider_2.maxPoint.x, collider_2.minPoint.y };
				Vec3 B = { collider_1.minPoint.x, collider_1.maxPoint.y };

				DynamicStaticCorner(A, B, ds_2);
			}
			//4
			else 
			{
				float s = collider_2.minPoint.x - collider_1.maxPoint.x - 1;
				if (body_1.velocity.Length() * body_1.Mass() > body_2.velocity.Length() * body_2.Mass())
				{
					ds_1.x *= 0.3f;
					ds_2.x = ds_1.x;
					ds_1.x += s;
				}
				else if (body_1.velocity.Length() * body_1.Mass() < body_2.velocity.Length() * body_2.Mass())
				{
					ds_2.x *= 0.3f;
					ds_1.x = ds_2.x;
					ds_2.x -= s;
				}
				else
				{
					if (1)
					{
						int ss = static_cast<int>(abs(s));
						int s1 = ss / 2, s2 = ss / 2;
						if (s < 0.0f)
							s1 *= -1;
						else
							s2 *= -1;
						ds_1.x = static_cast<float>(s1);
						ds_2.x = static_cast<float>(s2);
						// HD_CORE_INFO("{0}, {1}", ds_1.x, ds_2.x);
					}

					body_1.velocity.x = 0;
					body_2.velocity.x = 0;
				}
			}
		}
		else if (collider_1.minPoint.x > collider_2.maxPoint.x) 
		{
			//3
			if (collider_1.maxPoint.y < collider_2.minPoint.y)
			{
				ECS::Coordinator::GetComponent<Transform>(entt_2).Translate(ds_2);
				ds_2 = Vec3(0);

				Vec3 A = { collider_1.minPoint.x, collider_1.maxPoint.y };
				Vec3 B = { collider_2.maxPoint.x, collider_2.minPoint.y };

				DynamicStaticCorner(A, B, ds_1);
			}
			//9
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
				ECS::Coordinator::GetComponent<Transform>(entt_1).Translate(ds_1);
				ds_1 = Vec3(0);

				Vec3 A = collider_2.minPoint;
				Vec3 B = collider_1.maxPoint;

				DynamicStaticCorner(A, B, ds_2);
			}
			//6
			else 
			{
				float s = collider_2.maxPoint.x - collider_1.minPoint.x + 1;
				if (body_1.velocity.Length() * body_1.Mass() > body_2.velocity.Length() * body_2.Mass())
				{
					ds_1.x *= 0.3f;
					ds_2.x = ds_1.x;
					ds_1.x += s;
				}
				else if (body_1.velocity.Length() * body_1.Mass() < body_2.velocity.Length() * body_2.Mass())
				{
					ds_2.x *= 0.3f;
					ds_1.x = ds_2.x;
					ds_2.x -= s;
				}
				else
				{
					if (1)
					{
						int ss = static_cast<int>(abs(s));
						int s1 = ss / 2, s2 = ss / 2;
						if (s < 0.0f)
							s1 *= -1;
						else
							s2 *= -1;
						ds_1.x = static_cast<float>(s1);
						ds_2.x = static_cast<float>(s2);
						//HD_CORE_INFO("{0}, {1}", ds_1.x, ds_2.x);
					}

					body_1.velocity.x = 0;
					body_2.velocity.x = 0;
				}
			}
		}
		else
		{
			//2
			if (collider_1.maxPoint.y < collider_2.minPoint.y) 
			{
				HD_CORE_INFO("2");
				ECS::Coordinator::GetComponent<Transform>(entt_2).Translate(ds_2);
				ds_1.y = collider_2.minPoint.y - collider_1.maxPoint.y - 1;
				body_1.velocity.y = 0.f;
			}
			//8
			else if (collider_1.minPoint.y > collider_2.maxPoint.y) 
			{
				HD_CORE_INFO("8");
				ECS::Coordinator::GetComponent<Transform>(entt_1).Translate(ds_1);
				ds_2.y = collider_2.minPoint.y - collider_1.maxPoint.y + 1;
				body_2.velocity.y = 0.f;
			}
		}
	}
#endif

}