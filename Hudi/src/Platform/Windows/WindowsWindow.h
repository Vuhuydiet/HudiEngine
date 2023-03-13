#pragma once

#include <SDL.h>

#include "Hudi/Core/Window.h"

namespace Hudi {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		bool PollEvent(Event& e) override;
		// Get all the events in a frame
		void OnUpdate() override;

		inline WindowProps GetWindowProps() const override { return m_Properties; }

		inline uint32_t GetWidth() const override { return m_Properties.width; }
		inline uint32_t GetHeight() const override { return m_Properties.height; }
		
		SDL_Window* GetSDL_Window() { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		SDL_Window* m_Window = nullptr;

		WindowProps m_Properties;

		std::queue<Event> m_EventQueue;
	};

}