#pragma once
#include "hdpch.h"
#include <ECS.h>

namespace Hudi {

	struct Camera : public ECS::Component
	{
		void Translate(Vec2<int> ds);
		void Translate(int x, int y);
		void Translate(Vec3 ds);
		void Translate(int x, int y, int z);


		int x, y;
		uint32_t w, h;
	};

}