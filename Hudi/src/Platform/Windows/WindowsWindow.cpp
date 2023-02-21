#include "hdpch.h"
#include "WindowsWindow.h"

namespace Hudi {

	static bool s_SDLInitialised = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::OnUpdate()
	{
		EventManager::Clear();
		EventManager::OnUpdate();
	}


	void WindowsWindow::Init(const WindowProps& props)
	{
		int flags = 0;
		if (props.fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		m_Window = SDL_CreateWindow(
			props.title,
			props.xpos, props.ypos,
			props.width, props.height,
			flags
		);

		m_Title = props.title;
		WIDTH = props.width;
		HEIGHT = props.height;

		EventManager::Init();
	}

	void WindowsWindow::ShutDown()
	{
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}