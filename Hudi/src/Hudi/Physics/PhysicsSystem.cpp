#include "hdpch.h"
#include "PhysicsSystem.h"

#include "Hudi/Components/TransformComponent.h"
#include "Hudi/Components/RigidBody2DComponent.h"
#include "Hudi/Components/Collider2DComponent.h"
#include "Hudi/Components/BehaviourComponent.h"

#include "Collision2D.h"

namespace Hudi {

	PhysicsSystem::PhysicsSystem()
	{
		m_Signature.set(ECS::GetComponentID<Transform>());
		m_Signature.set(ECS::GetComponentID<RigidBody2D>());
	}

	void PhysicsSystem::OnUpdate()
	{
		std::unordered_map<ECS::Entity, Vec3> entities_ds;
		entities_ds.clear();

		for (auto entt : m_Entities)
		{
			auto& transform = ECS::Coordinator::GetComponent<Transform>(entt);
			auto& rigidBody = ECS::Coordinator::GetComponent<RigidBody2D>(entt);

			if (rigidBody.IsStatic())
			{
				if (ECS::Coordinator::HasComponent<BoxCollider2D>(entt))
					entities_ds[entt] = Vec3::Zero();
			}
			else if (rigidBody.IsDynamic())
			{
				rigidBody.force.y += rigidBody.Mass() * rigidBody.gravityScale;
				rigidBody.velocity += rigidBody.force / rigidBody.Mass() * Time::deltaTime();
			
				if (ECS::Coordinator::HasComponent<BoxCollider2D>(entt))
					entities_ds[entt] = rigidBody.velocity * Time::deltaTime();
				else
					transform.Translate(rigidBody.velocity * Time::deltaTime());

				rigidBody.force = Vec3::Zero();
			}
		}
		m_CollisionDetector.Update(entities_ds);
	}
}