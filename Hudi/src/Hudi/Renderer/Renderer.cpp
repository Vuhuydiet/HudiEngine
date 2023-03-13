#include "hdpch.h"
#include "Renderer.h"

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include "Hudi/ImGui/imgui_resources/imgui_impl_sdl2.h"
#include "Hudi/ImGui/imgui_resources/imgui_impl_opengl3.h"

#include "Hudi/Core/Application.h"

namespace Hudi {

	Renderer::Renderer()
	{
	}

	void Renderer::SetAttributes()
	{
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	}

	void Renderer::Init()
	{
		
	}

	void Renderer::ShutDown()
	{
	}

}