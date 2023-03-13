#include "hdpch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "Hudi/ImGui/imgui_resources/imgui_impl_sdl2.h"
#include "Hudi/ImGui/imgui_resources/imgui_impl_opengl3.h"

#include "Hudi/Core/Application.h"

namespace Hudi {
	
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

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Properties = props;
		m_Window = SDL_CreateWindow(
			props.title,
			props.xpos, props.ypos,
			props.width, props.height,
			props.flags
		);

		SDL_GLContext gl_context = SDL_GL_CreateContext(m_Window);
		SDL_GL_MakeCurrent(m_Window, gl_context);
		SDL_GL_SetSwapInterval(1);

		// Glad setup
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		EventManager::Init();
	}

	void WindowsWindow::ShutDown()
	{
		SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		SDL_Quit();
	}

	bool WindowsWindow::PollEvent(Event& e)
	{
		if (m_EventQueue.empty())
			return false;

		e = m_EventQueue.front();
		m_EventQueue.pop();
		return true;
	}

	void WindowsWindow::OnUpdate()
	{
		EventManager::Clear();
		// Clear the queue
		m_EventQueue = {};
		EventManager::OnUpdate(m_EventQueue);

		SDL_GetWindowPosition(m_Window, &m_Properties.xpos, &m_Properties.ypos);
		SDL_GetWindowSize(m_Window, &m_Properties.width, &m_Properties.height);
	}

}