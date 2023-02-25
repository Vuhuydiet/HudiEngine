#pragma once
#include "hdpch.h"
#include <ECS.h>

namespace Hudi {

	struct Camera : public ECS::Component
	{
		const char* ToString() const override { return "Camera"; }

		void Translate(Vec2<int> ds) {}
		void Translate(int x, int y) {}
		void Translate(Vec3 ds) {}
		void Translate(int x, int y, int z) {}


		enum Type { Perspective, Orthographic };
	public:
		Type type = Orthographic;
		uint32_t w, h;
		int C = 0;
	};

}