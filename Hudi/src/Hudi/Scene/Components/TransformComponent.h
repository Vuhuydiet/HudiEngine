#pragma once
#include "hdpch.h"

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hudi {

	class Transform : public Component
	{
	public:
		Transform() : scale(1.0f), localScale(1.0f) {}
		const char* ToString() const override { return "Transform"; }

		void Translate(float x, float y, float z) { position += Vec3(x, y, z); }
		void Translate(Vec3 v) { position += v; }

		glm::mat4 TransformationMatrix() const
		{
			return glm::translate(glm::mat4(1.0f), (glm::vec3)position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1.0f, 0.0f, 0.0f}) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0.0f, 1.0f, 0.0f}) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0.0f, 0.0f, 1.0f}) *
				glm::scale(glm::mat4(1.0f), (glm::vec3)scale);
		}

	public:
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;

		Vec3 localPosition;
		Vec3 localRotation;
		Vec3 localScale;

	};

}