#pragma once
#include "hdpch.h"

#include "Component.h"

namespace Hudi {

	struct RigidBody2D : public Component
	{
	public:
		enum class BodyType { Static, Dynamic };

	public:
		const char* ToString() const override { return "RigidBody2D"; }

		void AddForce(Vec3 in_force) { force += in_force; }
		void AddForce(float x, float y, float z) { force += Vec3(x, y, z); }

		bool IsStatic() { return bodyType == BodyType::Static; }
		bool IsDynamic() { return bodyType == BodyType::Dynamic; }

	public:
		BodyType bodyType = BodyType::Static;

		float mass = 1.0f;
		float gravityScale = 1.0f;

		Vec3 velocity;
		Vec3 force;
	};

}