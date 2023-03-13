#include "hdpch.h"
#include "RenderSystem.h"

#include "Hudi/Source/Time.h"
#include "Hudi/Components/TransformComponent.h"

#include "Hudi/Components/Collider2DComponent.h"

#include "Hudi/Scene/Scene.h"
#include "Hudi/Scene/SceneManager.h"

namespace Hudi {

	RenderSystem::RenderSystem()
	{
		m_Signature.set(ECS::GetComponentID<Transform>());
		m_Signature.set(ECS::GetComponentID<SpriteRenderer>());
	}

	RenderSystem::~RenderSystem()
	{
		HD_CORE_INFO("Destroyed Render System!");
	}

	void RenderSystem::Init(const WindowProps& props)
	{
		windowProps = props;
	}


	void RenderSystem::OnUpdate()
	{
		static int t = 0;
		t++;
		for (const auto& entt : m_Entities) 
		{
		}

		// Possibly camera updates and stuffs
		activeCamera = SceneManager::Get().GetActiveCamera();
	}

	void RenderSystem::OnRender()
	{
		if (!activeCamera.Exist() || !activeCamera.HasComponent<Camera>())
			return;

		//SDL_SetRenderDrawColor(Renderer::Game().GetSDL_Renderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
		//SDL_RenderClear(Renderer::Game().GetSDL_Renderer());

		DrawTextures();
		DrawBoxCollider2Ds();

		//SDL_RenderPresent(Renderer::Game().GetSDL_Renderer());
	}

	void RenderSystem::DrawTextures()
	{
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
				SDL_Rect dstRect = CalculateDstRect(entt);

				if (flip == SDL_FLIP_HORIZONTAL)
				{
					dstRect.x -= dstRect.w / 2;
				}

				//SDL_RenderCopyEx(Renderer::Game().GetSDL_Renderer(), sprite.m_Texture, &sprite.m_SrcRect, &dstRect, NULL, NULL, flip);
			}
		}
	}

	void RenderSystem::DrawBoxCollider2Ds()
	{
		for (auto& entt : m_Entities)
		{
			if (ECS::Coordinator::HasComponent<BoxCollider2D>(entt) &&
				ECS::Coordinator::GetComponent<BoxCollider2D>(entt).isDrawn)
			{
				const auto& entt_transform = ECS::Coordinator::GetComponent<Transform>(entt);
				const auto& entt_box = ECS::Coordinator::GetComponent<BoxCollider2D>(entt);

				const auto& camera_transform = activeCamera.GetComponent<Transform>();
				const auto& cam = activeCamera.GetComponent<Camera>();

				SDL_Rect r = ECS::Coordinator::GetComponent<BoxCollider2D>(entt);
				switch (cam.type)
				{
				case Camera::Perspective:
				{
					float C = cam.C * abs(camera_transform.position.z - entt_transform.position.z);
					float width = static_cast<float>(windowProps.width);
					float height = static_cast<float>(windowProps.height);
					float scale = width / height;

					float W = C * scale;
					float H = C;

					Vec3 screen_pos = entt_box.minPoint - camera_transform.position + Vec3(W / 2, H / 2, 0.0f);
					screen_pos *= width / W;

					r.w = static_cast<int>(r.w * width / W);
					r.h = static_cast<int>(r.h * height / H);
					r.x = static_cast<int>(screen_pos.x);
					r.y = static_cast<int>(screen_pos.y);

					break;
				}
				case Camera::Orthographic:
				{
					break;
				}
				}
				//Renderer::Game().DrawRect(r);
			}
		}
	}

	SDL_Rect RenderSystem::CalculateDstRect(ECS::Entity entt)
	{
		const auto& entt_transform = ECS::Coordinator::GetComponent<Transform>(entt);
		const auto& entt_sprite = ECS::Coordinator::GetComponent<SpriteRenderer>(entt);

		SDL_Rect dstRect = { 0, 0, 0, 0 };
		const auto& camera_transform = activeCamera.GetComponent<Transform>();
		const auto& cam = activeCamera.GetComponent<Camera>();
		switch (cam.type)
		{
			case Camera::Perspective:
			{
				float C = cam.C * abs(camera_transform.position.z - entt_transform.position.z);
				float width = static_cast<float>(windowProps.width);
				float height = static_cast<float>(windowProps.height);
				float scale = width / height;

				float W = C * scale;
				float H = C;

				Vec3 screen_pos = entt_transform.position - camera_transform.position + Vec3(W / 2, H / 2, 0.0f);
				screen_pos *= width / W;

				dstRect.w = static_cast<int>(entt_sprite.m_SrcRect.w * abs(entt_transform.scale.x) * width / W);
				dstRect.h = static_cast<int>(entt_sprite.m_SrcRect.h * abs(entt_transform.scale.y) * width / W);
				dstRect.x = static_cast<int>(screen_pos.x);
				dstRect.y = static_cast<int>(screen_pos.y);

				break;
			}
			case Camera::Orthographic:
			{
				dstRect.w = static_cast<int>(entt_sprite.m_SrcRect.w * abs(entt_transform.scale.x));
				dstRect.h = static_cast<int>(entt_sprite.m_SrcRect.h * abs(entt_transform.scale.y));
				dstRect.x = static_cast<int>(entt_transform.position.x);
				dstRect.y = static_cast<int>(entt_transform.position.y);
				break;
			}
		}

		return dstRect;
	}
}