#include "hdpch.h"
#include "RenderSystem.h"

#include "Hudi/Source/Time.h"
#include "Hudi/Components/TransformComponent.h"

#include "Hudi/Components/Collider2DComponent.h"

#include "Hudi/Scene/SceneManager.h"

namespace Hudi {

	RenderSystem::RenderSystem()
	{
		m_Signature.set(ECS::GetComponentID<Transform>());
		m_Signature.set(ECS::GetComponentID<SpriteRenderer>());
	}

	RenderSystem::~RenderSystem()
	{
		ShutDown();
	}

	void RenderSystem::Init(Ref<Window> window)
	{
		Renderer::Init(window->GetSDL_Window());
		m_Window = window;
	}

	void RenderSystem::ShutDown()
	{
		Renderer::ShutDown();
	}

	void RenderSystem::OnUpdate()
	{
		static int t = 0;
		t++;
		for (const auto& entt : m_Entities) 
		{
		}

		// Possibly camera updates and stuffs
	}

	void RenderSystem::OnRender()
	{
		activeCamera = SceneManager::GetActiveCamera();

		SDL_SetRenderDrawColor(Renderer::s_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(Renderer::s_Renderer);

		uint32_t maxOrder = 0;
		for (auto const& entity : m_Entities) 
		{
			maxOrder = std::max(maxOrder, ECS::Coordinator::GetComponent<SpriteRenderer>(entity).order);
		}

		for (uint32_t order = 0; order <= maxOrder; order++)
		{
			for (const auto& entt : m_Entities) 
			{
				const auto& transform = ECS::Coordinator::GetComponent<Transform>(entt);
				const auto& sprite = ECS::Coordinator::GetComponent<SpriteRenderer>(entt);

				if (sprite.order != order)
					continue;

				if (!sprite.m_Texture)
					continue;

				// Flip
				SDL_RendererFlip flip = transform.scale.x < 0.0f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

				// Dest Rect
				SDL_Rect dstRect;// = CalculateDstRect(entt);

				dstRect.w = static_cast<int>(sprite.m_SrcRect.w * abs(transform.scale.x));
				dstRect.h = static_cast<int>(sprite.m_SrcRect.h * abs(transform.scale.y));
				dstRect.x = static_cast<int>(transform.position.x);
				dstRect.y = static_cast<int>(transform.position.y);
				
				if (flip == SDL_FLIP_HORIZONTAL)
				{
					dstRect.x -= dstRect.w/2;
				}

				SDL_RenderCopyEx(Renderer::s_Renderer, sprite.m_Texture, &sprite.m_SrcRect, &dstRect, NULL, NULL, flip);
			}
		}

		for (auto& entt : m_Entities)
		{
			if (ECS::Coordinator::HasComponent<BoxCollider2D>(entt))
			{
				if (ECS::Coordinator::GetComponent<BoxCollider2D>(entt).isDrawn)
				{
					Renderer::DrawRect(ECS::Coordinator::GetComponent<BoxCollider2D>(entt));
				}
			}
		}

		SDL_RenderPresent(Renderer::s_Renderer);
	}

	SDL_Rect RenderSystem::CalculateDstRect(ECS::Entity entt)
	{
		const auto& transform = ECS::Coordinator::GetComponent<Transform>(entt);
		const auto& sprite = ECS::Coordinator::GetComponent<SpriteRenderer>(entt);

		SDL_Rect dstRect;

		switch (activeCamera->type)
		{
			case Camera::Perspective:
			{
				break;
			}
			case Camera::Orthographic:
			{
				dstRect.w = static_cast<int>(sprite.m_SrcRect.w * abs(transform.scale.x));
				dstRect.h = static_cast<int>(sprite.m_SrcRect.h * abs(transform.scale.y));
				dstRect.x = static_cast<int>(transform.position.x);
				dstRect.y = static_cast<int>(transform.position.y);
				break;
			}
		}

		return dstRect;
	}
}