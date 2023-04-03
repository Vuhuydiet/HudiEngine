#include "hdpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <SDL.h>

namespace Hudi {

	OpenGLContext::OpenGLContext(SDL_Window* window)
		: m_WindowHandle(window)
	{
		HD_CORE_ASSERT(!window, "No window has been created!");
	}

	void OpenGLContext::Init()
	{
		SDL_GLContext gl_context = SDL_GL_CreateContext(m_WindowHandle);
		SDL_GL_MakeCurrent(m_WindowHandle, gl_context);

		// Glad setup
		gladLoadGLLoader(SDL_GL_GetProcAddress);
	}

	void OpenGLContext::SetAttributes()
	{
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

		// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	}

}