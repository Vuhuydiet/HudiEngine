#pragma once

#include <ECS.h>

namespace Hudi {

	class Scene;

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();

		void OnUpdate(float dt);

	private:
		Scene* scene = nullptr;
		friend class Scene;
	};

}