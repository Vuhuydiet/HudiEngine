#include "hdpch.h"
#include "Application.h"

#include "Hudi/Core/Input.h"

#include <ECS.h>

#include "Hudi/ImGui/ImGuiLayer.h"

#include "Hudi/Renderer/Renderer.h"

namespace Hudi {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		if (s_Instance)
		{
			HD_CORE_ERROR("Already has an application!");
			exit(-1);
		}
		s_Instance = this;

		// Init Log
		Hudi::Log::Init();
		// Init SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		// Init ECS
		ECS::Init_Everything();
	}

	void Application::Init()
	{
		m_Running = true;

		Renderer::Get().SetAttributes();
		m_Window = Scope<Window>(Window::Create(WindowProperties()));
		Renderer::Get().Init();

		m_ImGuiLayer = new ImGuiLayer;
		m_LayerStack.PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
        Shutdown();
	}

    void Application::Shutdown()
    {
        Renderer::Get().ShutDown();

        // Destroy ECS
        ECS::Destroy_Everything();
        HD_CORE_INFO("Destroyed ECS in Application.cpp!");
    }

	WindowProps Application::WindowProperties()
	{
		return WindowProps();
	}

	void Application::OnEvent()
	{
		// ------------------------- //
		if (EventManager::Quit())
			CloseApplication();

		Event event;
		while (m_Window->PollEvent(event))
		{
			for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
			{
				if (event.handled)
					break;
				(*it)->OnEvent(event);
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::Run()
	{
        while (IsRunning())
        {
            //Timer timer(Time::deltaTime());

			m_Window->OnUpdate();
            OnEvent();

			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnImGuiRender();
			}
			m_ImGuiLayer->End();
        }
	}

}