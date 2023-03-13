#pragma once

#include "Core.h"
#include "Window.h"

#include "LayerStack.h"
#include "Hudi/ImGui/ImGuiLayer.h"

namespace Hudi {

	class HUDI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Init();
		void Shutdown();

		void OnEvent();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

		bool IsRunning() { return m_Running; }
		void CloseApplication() { m_Running = false; }

	protected:
		virtual WindowProps WindowProperties();

	private:
		void Run();
		bool m_Running = false;
	private:
		Scope<Window> m_Window = nullptr;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer = nullptr;
	private:
		static Application* s_Instance;

		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}

