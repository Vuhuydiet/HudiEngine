#pragma once

#include <ECS.h>
#include "Hudi/Core/Window.h"
#include "Renderer.h"
#include "SpriteRendererComponent.h"

#include "Hudi/Scene/CameraComponent.h"

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

		void SetActiveCamera(Camera* cam) { activeCamera = cam; }

	private:
		SDL_Rect CalculateDstRect(ECS::Entity entt);
	private:
		Ref<Window> m_Window = nullptr;

		Camera* activeCamera = nullptr;
	};

}