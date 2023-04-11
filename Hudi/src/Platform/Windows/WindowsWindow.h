#pragma once

#include "Hudi/Core/Window.h"
#include "Hudi/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace Hudi {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(float dt) override;
		virtual void SwapWindow() override;
		virtual void Resize() override;
		virtual bool IsMinimized() const override;
		virtual bool IsMaximized() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline const WindowProps& GetWindowProps() const override { return m_Properties; }

		inline uint32_t GetWidth() const override { return m_Properties.width; }
		inline uint32_t GetHeight() const override { return m_Properties.height; }
		
		virtual void* GetNativeWindow() override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		SDL_Window* m_Window = nullptr;
		Ref<GraphicsContext> m_GraphicsContext = nullptr;

		WindowProps m_Properties;
	};

}