#pragma once

#include "Core.h"
#include "Hudi/Events/Event.h"

namespace Hudi {

#define WINDOWPOS_CENTERED 0x2FFF0000u
	enum WindowFlags
	{
		WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
		WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
		WINDOW_SHOWN = 0x00000004,              /**< window is visible */
		WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
		WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
		WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
		WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
		WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
		WINDOW_MOUSE_GRABBED = 0x00000100,      /**< window has grabbed mouse input */
		WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
		WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
		WINDOW_FULLSCREEN_DESKTOP = (WINDOW_FULLSCREEN | 0x00001000),
		WINDOW_FOREIGN = 0x00000800,            /**< window not created by SDL */
		WINDOW_ALLOW_HIGHDPI = 0x00002000,      /**< window should be created in high-DPI mode if supported.
														 On macOS NSHighResolutionCapable must be set true in the
														 application's Info.plist for this to have any effect. */
		WINDOW_MOUSE_CAPTURE = 0x00004000,   /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
		WINDOW_ALWAYS_ON_TOP = 0x00008000,   /**< window should always be above others */
		WINDOW_SKIP_TASKBAR = 0x00010000,   /**< window should not be added to the taskbar */
		WINDOW_UTILITY = 0x00020000,   /**< window should be treated as a utility window */
		WINDOW_TOOLTIP = 0x00040000,   /**< window should be treated as a tooltip */
		WINDOW_POPUP_MENU = 0x00080000,   /**< window should be treated as a popup menu */
		WINDOW_KEYBOARD_GRABBED = 0x00100000,   /**< window has grabbed keyboard input */
		WINDOW_VULKAN = 0x10000000,   /**< window usable for Vulkan surface */
		WINDOW_METAL = 0x20000000,   /**< window usable for Metal view */

		WINDOW_INPUT_GRABBED = WINDOW_MOUSE_GRABBED /**< equivalent to SDL_WINDOW_MOUSE_GRABBED for compatibility */
	};

	struct WindowProps
	{
		const char* title;
		int xpos, ypos;
		int width, height;
		int flags;
		bool VSync;

		WindowProps();
		WindowProps(const char* title, int xpos, int ypos, int width, int height, int flags = 0, bool VSync = false);
	};

	class HUDI_API Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnEvent(Event& e) = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void SwapWindow() = 0;
		//virtual void Resize() = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual void ClearEventQueue() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const WindowProps& GetWindowProps() const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* Create(const WindowProps& props = WindowProps());
		
	};

}