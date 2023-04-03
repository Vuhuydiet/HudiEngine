#pragma once

#include "Hudi/Core/Core.h"
#include "Hudi/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace Hudi {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* window);

		virtual void Init() override;

		static void SetAttributes();

	private:
		SDL_Window* m_WindowHandle;
	};

}