#pragma once

#include "Core.h"
#include "Window.h"

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
		friend int ::main(int argc, char** argv);

		bool m_Running = false;

		Ref<Window> m_Window;

	private:
		Ref<PhysicsSystem> m_PhysicsSystem;
		Ref<RenderSystem> m_RenderSystem;
	};

	Application* CreateApplication();

}

