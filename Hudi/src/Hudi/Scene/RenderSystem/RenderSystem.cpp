#include "hdpch.h"
#include "RenderSystem.h"

#include "Hudi/Renderer/Renderer2D.h"

#include "Hudi/Scene/Components/TransformComponent.h"
#include "Hudi/Scene/Components/SpriteRendererComponent.h"
#include "Hudi/Scene/Components/CameraComponent.h"
#include "Hudi/Scene/Scene.h"

namespace Hudi {

	RenderSystem::RenderSystem()
	{
		SetNeededComponent<Transform, SpriteRenderer>();
	}

	void RenderSystem::OnUpdate(float dt, ECS::Entity camera)
	{
		RenderCommand::SetClearColor(169, 169, 169, 255);
		RenderCommand::Clear();

		if (world->Exists(camera))
		{
			const glm::mat4& cameraProjection = world->GetComponent<Camera>(camera)->GetProjection();
			const glm::mat4& cameraTranform = world->GetComponent<Transform>(camera)->Transformation();
			Renderer2D::BeginScene(cameraProjection, cameraTranform);
			for (const auto& entity : m_Entities)
			{
				if (!world->IsActive(entity))
					continue;

				const Transform& transform = *world->GetComponent<Transform>(entity);
				const SpriteRenderer& sprite = *world->GetComponent<SpriteRenderer>(entity);

				Quad quad;
				quad.transform = transform.Transformation();
				quad.size = sprite.size;
				quad.texture = sprite.texture;
				quad.color = sprite.color;
				Renderer2D::DrawQuad(quad);
			}
			Renderer2D::EndScene();
		}

	}

}