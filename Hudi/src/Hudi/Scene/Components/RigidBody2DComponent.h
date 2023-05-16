#pragma once
#include "hdpch.h"

#include "Component.h"

namespace Hudi {

	class Rigidbody2D : public Component
	{
	public:
		const char* ToString() const override { return "Rigidbody2D"; }
	public:
		enum class BodyType { Static, Kinematic, Dynamic };

	public:
		BodyType type = BodyType::Static;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;

	};

}