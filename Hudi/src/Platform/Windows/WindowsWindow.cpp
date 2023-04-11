#include "hdpch.h"
#include "WindowsWindow.h"

#include <SDL.h>

#include "Hudi/Core/Application.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hudi {
	
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		OpenGLContext::SetAttributes();
		Init(props);

		EventManager::SetWindowFn(HD_BIND_EVENT_FN(WindowsWindow::OnEvent));
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

		EventManager::Init();
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
		SDL_Event event = e;
		switch (event.type)
		{
		case WINDOW_EVENT:
			switch (event.window.event)
			{
			case WINDOWEVENT_RESIZED:
			case WINDOWEVENT_SIZE_CHANGED:
				SDL_GetWindowSize(m_Window, &m_Properties.width, &m_Properties.height);
				break;
			}
			break;
		}
	}

	void WindowsWindow::OnUpdate(float dt)
	{
		EventManager::Clear();
		EventManager::OnUpdate();

		// TODO: currently defining the window is freezing when dt > 0.5f seconds
		// Find another way in the future
		if (dt > 0.5f)
		{
			EventManager::Reset();
		}

		SDL_GetWindowPosition(m_Window, &m_Properties.xpos, &m_Properties.ypos);
	}

	void WindowsWindow::SwapWindow()
	{
		SDL_GL_SwapWindow(m_Window);
	}

	void WindowsWindow::Resize()
	{
		SDL_GetWindowSize(m_Window, &m_Properties.width, &m_Properties.height);
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