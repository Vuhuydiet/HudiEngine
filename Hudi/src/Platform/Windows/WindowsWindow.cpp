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

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Properties = props;
		m_Window = SDL_CreateWindow(
			props.title,
			props.xpos, props.ypos,
			props.width, props.height,
			props.flags
		);

		EventManager::Init();
	}

	void WindowsWindow::ShutDown()
	{
		HD_CORE_INFO("Destroyed window!");
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
	}

}