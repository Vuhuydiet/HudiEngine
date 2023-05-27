#include "hdpch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Hudi {

	WindowProps::WindowProps()
		: title("unknown"),
		xpos(0), ypos(0),
		width(800), height(600),
		flags(0),
		VSync(false)
	{}

	WindowProps::WindowProps(const char* title, int xpos, int ypos, int width, int height, int flags, bool VSync)
		: title(title), xpos(xpos), ypos(ypos), width(width), height(height), flags(flags), VSync(VSync) {}


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