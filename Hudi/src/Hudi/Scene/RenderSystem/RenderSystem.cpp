#include "hdpch.h"
#include "RenderSystem.h"

#include "Hudi/Scene/Components/Component.h"

#include "Hudi/Renderer/Renderer2D.h"

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

		ECS::Entity cameraEntt = 0;
		for (const auto entt : world->View<Camera>())
		{
			cameraEntt = entt;
		}
		if (!cameraEntt)
		{
			return;
		}

		const glm::mat4& cameraProjection = world->GetComponent<Camera>(cameraEntt)->GetProjection();
		const glm::mat4& cameraTranform = world->GetComponent<Transform>(cameraEntt)->TransformationMatrix();
		Renderer2D::BeginScene(cameraProjection, cameraTranform);
		for (const auto& entt : m_Entities)
		{
			const Transform& transform = *world->GetComponent<Transform>(entt);
			const SpriteRenderer& sprite = *world->GetComponent<SpriteRenderer>(entt);

			Quad quad;
			quad.transform = transform.TransformationMatrix();
			quad.size = sprite.size;
			quad.texture = sprite.texture;
			quad.color = sprite.color;
			Renderer2D::DrawQuad(quad);
		}
		Renderer2D::EndScene();
	}

}