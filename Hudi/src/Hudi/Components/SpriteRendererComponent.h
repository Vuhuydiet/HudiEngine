#pragma once

#include "Hudi/Renderer/Renderer.h"

#include <ECS.h>
#include <SDL.h>
#include <SDL_image.h>

namespace Hudi {

	struct SpriteRenderer : public ECS::Component
	{
		SpriteRenderer()
			: order(0)
		{
		}
		~SpriteRenderer()
		{
		}
		const char* ToString() const override { return "SpriteRenderer"; }

		void SetFile(const char* image)
		{
		}

		void SetSourceRect(int x, int y, int w, int h)
		{
		}

		const char* GetFilePath() { return m_FilePath; }

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