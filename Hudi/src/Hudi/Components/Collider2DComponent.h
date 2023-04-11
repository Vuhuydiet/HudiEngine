#pragma once
#include "hdpch.h"

#include "Component.h"

namespace Hudi {

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