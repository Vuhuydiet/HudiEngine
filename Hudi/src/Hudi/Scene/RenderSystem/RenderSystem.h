#pragma once

#include <ECS.h>
#include "Hudi/Scene/GameObject.h"

namespace Hudi {

	class Scene;

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();

		void OnUpdate(float dt, ECS::Entity camera);

	};

}