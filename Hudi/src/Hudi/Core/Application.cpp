#include "hdpch.h"
#include "Application.h"

#include "Hudi/Core/Input.h"

#include "Hudi/Renderer/Renderer.h"

#include "Hudi/ImGui/ImGuiLayer.h"

#include <ECS.h>

namespace Hudi {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_LastFrameTime(0.0f),
		m_Running(true),
		m_Window(nullptr), m_ImGuiLayer(nullptr)
	{
		HD_CORE_ASSERT(s_Instance, "Already has an application!");
		s_Instance = this;

		// Init ECS
		ECS::Init_Everything();

		m_Window = Scope<Window>(Window::Create(WindowProperties()));
		EventManager::SetCallBackFn(HD_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		////////////////////////////////////////////////////////////////////
		///--------------------------------------------------------------///
		////////////////////////////////////////////////////////////////////

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::Init()
	{
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Shutdown()
	{
		ECS::Destroy_Everything();
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if (e.handled)
				break;
			(*it)->OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch(QUIT, HD_BIND_EVENT_FN(Application::OnQuitEvent));
		dispatcher.Dispatch(WINDOW_EVENT, HD_BIND_EVENT_FN(Application::OnWindowEvent));
	}

	void Application::Run()
	{
		while (IsRunning())
		{
			m_Window->OnUpdate();

			float currentTime = m_Clock.GetSeconds();
			float dt = currentTime - m_LastFrameTime;
			m_LastFrameTime = currentTime;

			if (!m_Window->IsMinimized()) {
				for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
				{
					(*it)->OnUpdate(dt);
				}
			}

			m_ImGuiLayer->Begin();
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
	}

	WindowProps Application::WindowProperties()
	{
		return WindowProps();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnQuitEvent(Event& e)
	{
		CloseApplication();
	}

	void Application::OnWindowEvent(Event& e)
	{
		SDL_Event event = e;
		switch (event.window.event)
		{
		case WINDOWEVENT_RESIZED:
		case WINDOWEVENT_SIZE_CHANGED:
			Renderer::OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());
			break;
		}
	}
}