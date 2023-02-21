#pragma once

#include "Hudi/Core/Window.h"

#include <SDL.h>


namespace Hudi {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// Gets all the events in a frame
		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return WIDTH; }
		inline uint32_t GetHeight() const override { return HEIGHT; }
		
		SDL_Window* GetSDL_Window() { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		SDL_Window* m_Window = nullptr;

		const char* m_Title;
		uint32_t WIDTH, HEIGHT;

	};

}