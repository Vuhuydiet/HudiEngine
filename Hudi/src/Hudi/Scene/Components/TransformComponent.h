#pragma once
#include "hdpch.h"

namespace Hudi {

	class Transform
	{
	public:
		Transform();

		void Translate(float x, float y, float z);
		void Translate(const Vec3& v);

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