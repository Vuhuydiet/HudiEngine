#include "hdpch.h"
#include "WindowsWindow.h"

#include "Hudi/Core/Application.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <SDL.h>

namespace Hudi {
	
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		OpenGLContext::SetAttributes();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Properties = props;
		m_Window = SDL_CreateWindow(
			props.title,
			props.xpos, props.ypos,
			props.width, props.height,
			props.flags
		);
		HD_CORE_ASSERT(!m_Window, "Window creation failed!");

		m_GraphicsContext = NewRef<OpenGLContext>(m_Window);
		m_GraphicsContext->Init();

		SetVSync(m_Properties.VSync);
	}

	void WindowsWindow::ShutDown()
	{
		SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		SDL_Quit();
	}

	void WindowsWindow::OnEvent(Event& e)
	{
		switch (e.type())
		{
		case WINDOW_EVENT:
			switch (e.WindowType())
			{
			case WINDOWEVENT_RESIZED:
			case WINDOWEVENT_SIZE_CHANGED:
				SDL_GetWindowSize(m_Window, &m_Properties.width, &m_Properties.height);
				break;
			case WINDOWEVENT_MOVED:
				SDL_GetWindowPosition(m_Window, &m_Properties.xpos, &m_Properties.ypos);
				break;
			}
			break;
		}
	}

	void WindowsWindow::OnUpdate(float dt)
	{
		EventManager::Clear();
		EventManager::OnUpdate();
	}

	void WindowsWindow::SwapWindow()
	{
		SDL_GL_SwapWindow(m_Window);
	}

	/*void WindowsWindow::Resize()
	{
		SDL_GetWindowSize(m_Window, &m_Properties.width, &m_Properties.height);
	}*/

	void WindowsWindow::ClearEventQueue() const
	{
		EventManager::ClearEventQueue();
		EventManager::Reset();
	}

	bool WindowsWindow::IsMinimized() const
	{
		uint32_t flags = SDL_GetWindowFlags(m_Window);
		return flags & SDL_WINDOW_MINIMIZED;
	}

	bool WindowsWindow::IsMaximized() const
	{
		uint32_t flags = SDL_GetWindowFlags(m_Window);
		return flags & SDL_WINDOW_MAXIMIZED;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		SDL_GL_SetSwapInterval(enabled ? 1 : 0);
		m_Properties.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Properties.VSync;
	}

}