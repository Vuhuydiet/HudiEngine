#include "hdpch.h"
#include "Physics2DSystem.h"

#include "Hudi/Scene/Components/TransformComponent.h"
#include "Hudi/Scene/Components/Rigidbody2DComponent.h"
#include "Hudi/Scene/Components/Collider2DComponent.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Hudi {

	b2BodyType HDBodyTypeTob2BodyType(Rigidbody2D::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2D::BodyType::Static: return b2_staticBody;
			case Rigidbody2D::BodyType::Kinematic: return b2_kinematicBody;
			case Rigidbody2D::BodyType::Dynamic: return b2_dynamicBody;
		}
		HD_CORE_ASSERT(false, "BodyType not supported!");
		return b2_staticBody;
	}

	Physics2DSystem::Physics2DSystem()
	{
		SetNeededComponent<Transform, Rigidbody2D>();
	}

	void Physics2DSystem::OnEntityAdded(ECS::Entity entity)
	{
		if (m_OnRuntime && world->IsActive(entity))
		{
			AddEntityPhysicsDef(entity);
		}
	}

	void Physics2DSystem::OnEntityRemoved(ECS::Entity entity)
	{
		if (m_OnRuntime && world->IsActive(entity))
		{
			RemoveEntityPhyicsDef(entity);
		}
	}

	void Physics2DSystem::Begin()
	{
		m_OnRuntime = true;

		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		for (auto entity : m_Entities)
		{
			if (!world->IsActive(entity))
				continue;
			AddEntityPhysicsDef(entity);
		}
	}

	void Physics2DSystem::End()
	{
		m_OnRuntime = false;

		for (auto entity : m_Entities)
		{
			if (!world->IsActive(entity))
				continue;
			RemoveEntityPhyicsDef(entity);
		}

		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Physics2DSystem::OnUpdate(float dt)
	{
		const int velocityIterations = 8;
		const int positionIterations = 3;
		m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

		for (auto entity : m_Entities)
		{
			if (!world->IsActive(entity))
				continue;

			auto& transform = *world->GetComponent<Transform>(entity);
			auto& rb2 = *world->GetComponent<Rigidbody2D>(entity);

			b2Body* body = (b2Body*)rb2.runtimeBody;
			const auto& position = body->GetPosition();
			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.rotation.z = glm::degrees(body->GetAngle());
		}
	}

	void Physics2DSystem::AddEntityPhysicsDef(ECS::Entity entity)
	{
		auto& transform = *world->GetComponent<Transform>(entity);
		auto& rb2 = *world->GetComponent<Rigidbody2D>(entity);

		b2BodyDef bodyDef;
		bodyDef.type = HDBodyTypeTob2BodyType(rb2.type);
		auto [xpos, ypos, zpos] = transform.position;
		bodyDef.position.Set(xpos, ypos);
		bodyDef.angle = glm::radians(transform.rotation.z);
		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2.fixedRotation);
		rb2.runtimeBody = body;

		if (world->HasComponent<BoxCollider2D>(entity))
		{
			auto& box2 = *world->GetComponent<BoxCollider2D>(entity);

			b2PolygonShape shape;
			shape.SetAsBox(box2.size.x * transform.scale.x, box2.size.y * transform.scale.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = box2.density;
			fixtureDef.friction = box2.friction;
			fixtureDef.restitution = box2.restitution;
			fixtureDef.restitutionThreshold = box2.restitutionThreshold;
			body->CreateFixture(&fixtureDef);
		}
	}

	void Physics2DSystem::RemoveEntityPhyicsDef(ECS::Entity entity)
	{
		auto& transform = *world->GetComponent<Transform>(entity);
		auto& rb2 = *world->GetComponent<Rigidbody2D>(entity);

		if (world->HasComponent<BoxCollider2D>(entity))
		{
			auto& box2 = *world->GetComponent<BoxCollider2D>(entity);
			((b2Body*)rb2.runtimeBody)->DestroyFixture((b2Fixture*)box2.runtimeFixture);
			box2.runtimeFixture = nullptr;
		}
		m_PhysicsWorld->DestroyBody((b2Body*)rb2.runtimeBody);
		rb2.runtimeBody = nullptr;
	}

}