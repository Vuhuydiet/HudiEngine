#include "hdpch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

#include <SDL.h>

namespace Hudi {

	WindowProps::WindowProps()
		: title("unknown"),
		xpos(SDL_WINDOWPOS_CENTERED), ypos(SDL_WINDOWPOS_CENTERED),
		width(800), height(600),
		flags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
		VSync(false)
	{}

	Window* Window::Create(const WindowProps& props)
	{
#ifdef HD_PLATFORM_WINDOWS
		return new WindowsWindow(props);
#else
		HD_CORE_ASSERT(true, "Unknown platform!");
		return nullptr;
#endif
	}

}