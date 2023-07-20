#pragma once
#include "hdpch.h"

namespace Hudi {

	class Rigidbody2D
	{
	public:
		enum class BodyType { Static, Kinematic, Dynamic };
	public:
		void AddForce(const Vec3& force);

	public:
		BodyType type = BodyType::Static;

		Vec3 velocity;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;

	};

}