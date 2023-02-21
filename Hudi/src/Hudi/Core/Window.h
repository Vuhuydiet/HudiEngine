#pragma once

#include "Core.h"

#include "Hudi/Events/Event.h"

#include <SDL.h>

namespace Hudi {

	struct WindowProps
	{
		const char* title;
		int xpos, ypos;
		unsigned int width, height;
		bool fullscreen;

		WindowProps()
			: title("unknown"),
			xpos(SDL_WINDOWPOS_CENTERED), ypos(SDL_WINDOWPOS_CENTERED),
			width(800), height(600),
			fullscreen(false)
		{}
	};

	class HUDI_API Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual SDL_Window* GetSDL_Window() { return nullptr; };

		static Window* Create(const WindowProps& props = WindowProps());
		
	};

}