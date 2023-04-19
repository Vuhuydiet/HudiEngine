#pragma once

#include <ECS.h>

namespace Hudi {

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();

		void OnUpdate(float dt);

	private:
	};

}