#pragma once

#include "Core.h"

#include "Hudi/Events/Event.h"

struct SDL_Window;

namespace Hudi {

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