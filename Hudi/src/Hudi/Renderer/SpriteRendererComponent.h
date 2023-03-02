#pragma once

#include "Hudi/Core/Renderer.h"

#include <ECS.h>
#include <SDL.h>
#include <SDL_image.h>

namespace Hudi {

	struct SpriteRenderer : public ECS::Component
	{
		SpriteRenderer()
			: order(0)
		{
			m_SrcRect.x = m_SrcRect.y = m_SrcRect.w = m_SrcRect.h = 0;
		}
		~SpriteRenderer()
		{
			if (m_Texture)
				Renderer::DestroyTexture(m_Texture);
		}
		const char* ToString() const override { return "SpriteRenderer"; }

		void SetFile(const char* image)
		{
			if (!image)
				return;

			m_FilePath = image;
			Renderer::DestroyTexture(m_Texture);
			m_Texture = Renderer::CreateTexture(m_FilePath);

			SDL_Surface* tmpSur = IMG_Load(image);
			m_SrcRect.h = tmpSur->h;
			m_SrcRect.w = tmpSur->w;
			m_SrcRect.x = m_SrcRect.y = 0;

			SDL_FreeSurface(tmpSur);
		}

		void SetSourceRect(int x, int y, int w, int h)
		{
			m_SrcRect.x = x;
			m_SrcRect.y = y;
			m_SrcRect.w = w;
			m_SrcRect.h = h;
		}

	public:
		uint32_t order;

	private:
		SDL_Texture* m_Texture = nullptr;
		SDL_Rect m_SrcRect;
		const char* m_FilePath = nullptr;

	private:
		friend class RenderSystem;
	};

}