#pragma once

#include "Component.h"

#include <glm/glm.hpp>

namespace Hudi {

	class BoxCollider2D : public Component
	{
	public:
		virtual const char* ToString() const override { return "BoxCollider2D"; }

		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;
	};

#ifdef COLLIDER
	class Collider : public Component
	{
	public:
		virtual ~Collider() = default;
		virtual const char* ToString() const override { return "Collider"; }

		virtual void Translate(float x, float y) = 0;
		virtual void Translate(Vec3 v) = 0;

		virtual bool TestCollision(Collider* cb) = 0;

	public:
		static void AddTag(const char* _tag) { s_AvailableTags.insert(_tag); }
	public:
		const char* tag = "default";
	private:
		static std::unordered_set<const char*> s_AvailableTags;
	};

	class BoxCollider2D : public Collider
	{
	public:
		const char* ToString() const override { return "BoxCollider"; }

		void Translate(float x, float y) override
		{
			minPoint += Vec3(x, y);
			maxPoint += Vec3(x, y);
		}
		void Translate(Vec3 v)
		{
			minPoint += v;
			maxPoint += v;
		}

		bool TestCollision(Collider* cb) override;

	
		bool isDrawn = false;

		Vec3 minPoint;
		Vec3 maxPoint;

	public:
	};

	class CircleCollider2D : public Collider
	{
	public:
		const char* ToString() const override { return "CircleCollider"; }

		void Translate(float x, float y) override
		{
			center += Vec3(x, y);
		}
		void Translate(Vec3 v)
		{
			center += v;
		}

		bool TestCollision(Collider* cb) override;

	public:
		Vec3 center;
		int radius;
	};
#endif

}