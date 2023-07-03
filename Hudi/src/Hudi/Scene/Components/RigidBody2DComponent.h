#pragma once
#include "hdpch.h"

#include "Component.h"

namespace Hudi {

	class Rigidbody2D
	{
	public:
		virtual ~Rigidbody2D() = default;

		enum class BodyType { Static, Kinematic, Dynamic };
	public:
		BodyType type = BodyType::Static;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;

	};

}