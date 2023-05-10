#pragma once
#include "hdpch.h"

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hudi {

	class Transform : public Component
	{
	public:
		Transform();
		const char* ToString() const override { return "Transform"; }

		void Translate(float x, float y, float z);
		void Translate(Vec3 v);

		glm::mat4 Transformation() const;

	public:
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;

		Vec3 localPosition;
		Vec3 localRotation;
		Vec3 localScale;

	};

}