#pragma once

#include <ECS.h>
#include "Renderer.h"
#include "SpriteRendererComponent.h"

namespace Hudi {

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();
		~RenderSystem();


		virtual void OnUpdate() override;
		virtual void OnRender();

		void Init(SDL_Window* window);
		void ShutDown();
	};

}