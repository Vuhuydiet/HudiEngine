#pragma once

#include <SDL.h>

#include "Hudi/Core/Window.h"

namespace Hudi {

	class Renderer
	{
	public:
		static void Init(SDL_Window* window);
		static void ShutDown();

		static SDL_Texture* CreateTexture(const char* image);
		static void DestroyTexture(SDL_Texture* text);

		static void DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);

		static void DrawRect(SDL_Rect r);
		static void DrawRect(Vec2<int> min, Vec2<int> max);
		static void DrawRect(int x1, int y1, int x2, int y2);

	private:
		static SDL_Renderer* s_Renderer;
		friend class RenderSystem;
	};

}