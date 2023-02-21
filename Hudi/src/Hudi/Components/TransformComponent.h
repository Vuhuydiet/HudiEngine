#pragma once
#include "hdpch.h"

#include <ECS.h>

#include "Hudi/Source/Vec2.h"
#include "Hudi/Source/Vec3.h"

namespace Hudi {

	struct Transform : public ECS::Component
	{
		Transform() :
			scale(1.0f, 1.0f, 1.0f),
			localScale(1.0f, 1.0f, 1.0f)
		{}
		const char* ToString() const override { return "Transform"; }

		void Translate(float x, float y, float z) { position += Vec3(x, y, z); }
		void Translate(Vec3 v) { position += v; }

	public:
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;

		Vec3 localPosition;
		Vec3 localRotation;
		Vec3 localScale;

	private:
		Transform* m_Parent = this;
		std::vector<Transform*> m_Childen;
		friend class GameObject;
	};

}