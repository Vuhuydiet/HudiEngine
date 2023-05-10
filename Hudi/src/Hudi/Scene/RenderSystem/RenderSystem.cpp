#include "hdpch.h"
#include "RenderSystem.h"

#include "Hudi/Renderer/Renderer2D.h"

#include "Hudi/Scene/Components/Component.h"
#include "Hudi/Scene/Scene.h"

namespace Hudi {

	RenderSystem::RenderSystem()
	{
		m_Signature.set(ECS::GetComponentID<Transform>());
		m_Signature.set(ECS::GetComponentID<SpriteRenderer>());
	}

	void RenderSystem::OnUpdate(float dt)
	{
		RenderCommand::SetClearColor(169, 169, 169, 255);
		RenderCommand::Clear();

		GameObject camera = scene->GetPrimaryCamera();
		if (camera.Valid())
		{
			const glm::mat4& cameraProjection = world->GetComponent<Camera>(camera.GetEntityID())->GetProjection();
			const glm::mat4& cameraTranform = world->GetComponent<Transform>(camera.GetEntityID())->Transformation();
			Renderer2D::BeginScene(cameraProjection, cameraTranform);
			for (const auto& entt : m_Entities)
			{
				const Transform& transform = *world->GetComponent<Transform>(entt);
				const SpriteRenderer& sprite = *world->GetComponent<SpriteRenderer>(entt);

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