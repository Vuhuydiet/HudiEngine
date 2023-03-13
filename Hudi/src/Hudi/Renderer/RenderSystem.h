#pragma once

#include <ECS.h>
#include "Hudi/Core/Window.h"
#include "Hudi/Renderer/Renderer.h"
#include "Hudi/Components/SpriteRendererComponent.h"

#include "Hudi/Scene/CameraComponent.h"
#include "Hudi/Scene/GameObject.h"

namespace Hudi {

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem();
		~RenderSystem();

		void OnUpdate() override;
		void OnRender();

		void Init(const WindowProps& props);

		void SetActiveCamera(const GameObject& cam) { activeCamera = cam; }

	private:
		void DrawTextures();
		void DrawBoxCollider2Ds();

		SDL_Rect CalculateDstRect(ECS::Entity entt);
	private:
		WindowProps windowProps;

		GameObject activeCamera;
	};

}