#pragma once

#include <ECS.h>
#include "Hudi/Core/Window.h"
#include "Renderer.h"
#include "SpriteRendererComponent.h"

namespace Hudi {

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();
		~RenderSystem();

		void OnUpdate() override;
		void OnRender();

		void Init(Ref<Window> window);
		void ShutDown();

	private:
		Ref<Window> m_Window = nullptr;
	};

}