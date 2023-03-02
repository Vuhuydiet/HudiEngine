#pragma once

#include "Core.h"
#include "Window.h"
#include "Renderer.h"

#include "LayerStack.h"

#include "Hudi/Scene/SceneManager.h"

#include "Hudi/Physics/PhysicsSystem.h"
#include "Hudi/Renderer/RenderSystem.h"

namespace Hudi {

	class HUDI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Init();
		void ShutDown();

		virtual void Preprocess() = 0;

		void OnAwake();
		void OnEvent();
		void OnUpdate();
		void OnRender();

		bool IsRunning() { return m_Running; }

		void CloseApplication() { m_Running = false; }

	protected:
		virtual WindowProps WindowProperties();

	private:
		void Run();
		bool m_Running = false;

		Ref<Window> m_Window = nullptr;
		LayerStack m_LayerStack;
	private:
		PhysicsSystem* m_PhysicsSystem;
		RenderSystem* m_RenderSystem;

		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}

