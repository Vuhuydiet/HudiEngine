#pragma once

#include "Core.h"

#include "Window.h"
#include "Clock.h"

#include "LayerStack.h"
#include "Hudi/ImGui/ImGuiLayer.h"

namespace Hudi {

	class HUDI_API Application
	{
	public:
		Application(WindowProps props = WindowProps());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		bool IsRunning() const { return m_Running; }
		void Close() { m_Running = false; }

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		void OnQuitEvent(Event& e);
		void OnWindowEvent(Event& e);

	private:
		void Run();
		bool m_Running;

		Clock m_Clock;
		float m_LastFrameTime;
	private:
		Scope<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}

