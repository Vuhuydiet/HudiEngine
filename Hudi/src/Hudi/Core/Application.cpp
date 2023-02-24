#include "hdpch.h"
#include "Application.h"

#include "Hudi/Core/Input.h"

#include <SDL.h>
#include <ECS.h>

namespace Hudi {

	Application::Application()
	{
		// Init Log
		Hudi::Log::Init();

		// Init SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Init ECS
		ECS::Init_Everything();

		m_PhysicsSystem = ECS::Coordinator::RegisterSystem<PhysicsSystem>();
	}

	void Application::Init()
	{
		// Create Window
		m_Window = Scope<Window>(Window::Create(WindowProperties()));

		// Init Render System
		m_RenderSystem = ECS::Coordinator::RegisterSystem<RenderSystem>();
		m_RenderSystem->Init(m_Window);

		// Start the app
		m_Running = true;

		// Create default MainMenu scene
		SceneManager::CreateNewScene("MainMenu");
	}

	Application::~Application()
	{
		// Destroy ECS
		ECS::Destroy_Everything();
		HD_CORE_INFO("Destroyed ECS in Application.cpp !");
	}

	void Application::OnAwake()
	{
		// Awake Behaviours
		ECS::Coordinator::OnAwakeComponents();
	}

	void Application::OnEvent()
	{
		m_Window->OnUpdate();


		// ------------------------- //
		if (Input::Quit())
			CloseApplication();

	}

	void Application::OnUpdate()
	{
		// Update Behaviours
		ECS::Coordinator::OnUpdateComponents();

		// Update Systems
		ECS::Coordinator::OnUpdateSystem<PhysicsSystem>();
		ECS::Coordinator::OnUpdateSystem<RenderSystem>();
	}

	void Application::OnRender()
	{
		m_RenderSystem->OnRender();
	}

	void Application::Run()
	{
		Preprocess();

		SceneManager::GetActiveScene().BeginScene();
		while (IsRunning())
		{
			Timer timer(Time::deltaTime());

			OnAwake();
			OnEvent();
			OnUpdate();
			OnRender();

			ECS::Coordinator::OnUpdate();
		}
		SceneManager::GetActiveScene().EndScene();
	}

	WindowProps Application::WindowProperties()
	{
		return WindowProps();
	}

}