#include "hdpch.h"
#include "Renderer.h"

#include <SDL_image.h>

namespace Hudi {

	SDL_Renderer* Renderer::s_Renderer = nullptr;

	void Renderer::Init(SDL_Window* window)
	{
		s_Renderer = SDL_CreateRenderer(window, -1, 0);
	}

	void Renderer::ShutDown()
	{
		SDL_DestroyRenderer(s_Renderer);
	}

	SDL_Texture* Renderer::CreateTexture(const char* image)
	{
		std::fstream test;
		test.open(image);
		if (test.fail())
		{
			HD_CORE_WARN("Creating texture from an unexisted file!");
			test.close();
			return nullptr;
		}
		test.close();
		
		SDL_Surface* tempSur = IMG_Load(image);
		SDL_Texture* text = SDL_CreateTextureFromSurface(s_Renderer, tempSur);
		SDL_FreeSurface(tempSur);
		return text;
	}

	void Renderer::DestroyTexture(SDL_Texture* text)
	{
		SDL_DestroyTexture(text);
	}
	
	void Renderer::DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
	{
		SDL_SetRenderDrawColor(s_Renderer, r, g, b, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(s_Renderer, x1, y1, x2, y2);
	}

	void Renderer::DrawRect(SDL_Rect r)
	{
		SDL_SetRenderDrawColor(s_Renderer, 57, 255, 20, SDL_ALPHA_OPAQUE);
		
		Vec2<int> min = { r.x, r.y };
		Vec2<int> max = { r.x + r.w - 1, r.y + r.h - 1 };
		DrawRect(min, max);
	}

	void Renderer::DrawRect(Vec2<int> min, Vec2<int> max)
	{
		SDL_SetRenderDrawColor(s_Renderer, 57, 255, 20, SDL_ALPHA_OPAQUE);

		SDL_RenderDrawLine(s_Renderer, min.x, min.y, max.x, min.y);
		SDL_RenderDrawLine(s_Renderer, min.x, min.y, min.x, max.y);
		SDL_RenderDrawLine(s_Renderer, min.x, max.y, max.x, max.y);
		SDL_RenderDrawLine(s_Renderer, max.x, min.y, max.x, max.y);
	}

	void Renderer::DrawRect(int x1, int y1, int x2, int y2)
	{
		DrawRect(Vec2<int>(x1, y1), Vec2<int>(x2, y2));
	}
}