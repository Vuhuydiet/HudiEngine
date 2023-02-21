#pragma once
#include "hdpch.h"

#include <ECS.h>

namespace Hudi {

	struct RigidBody2D : public ECS::Component
	{
	public:
		const char* ToString() const override { return "RigidBody2D"; }

		void SetMass(float m) { m_Mass = std::max(m, 0.0001f); }
		float Mass() const { return m_Mass; }

		void AddForce(Vec3 in_force) { force += in_force; }
		void AddForce(float x, float y, float z) { force += Vec3(x, y, z); }

		bool IsStatic() { return bodyType == BodyType::Static; }
		bool IsDynamic() { return bodyType == BodyType::Dynamic; }

		enum class BodyType { Static, Dynamic };

	public:
		BodyType bodyType = BodyType::Static;

		float gravityScale = 1.0f;

		Vec3 velocity;
		Vec3 force;

	private:
		float m_Mass = 1.0f;
	};

}