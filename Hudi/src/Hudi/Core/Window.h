#pragma once

#include "Core.h"

#include "Hudi/Events/Event.h"

#include <SDL.h>

namespace Hudi {

	struct WindowProps
	{
		const char* title;
		int xpos, ypos;
		int width, height;
		int flags;

		WindowProps()
			: title("unknown"),
			xpos(SDL_WINDOWPOS_CENTERED), ypos(SDL_WINDOWPOS_CENTERED),
			width(800), height(600),
			flags(0)
		{}
	};

	class HUDI_API Window
	{
	public:
		virtual ~Window() = default;

		virtual bool PollEvent(Event& e) = 0;
		virtual void OnUpdate() = 0;

		virtual WindowProps GetWindowProps() const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual SDL_Window* GetSDL_Window() { return nullptr; };

		static Window* Create(const WindowProps& props = WindowProps());
		
	};

}