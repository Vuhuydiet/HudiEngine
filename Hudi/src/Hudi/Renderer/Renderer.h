#pragma once

#include "Hudi/Core/Window.h"

namespace Hudi {

	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator= (const Renderer&) = delete;

		void SetAttributes();

		void Init();
		void ShutDown();

		static Renderer& Get() { static Renderer s_Instance; return s_Instance; }
	private:

		Renderer();
	};

}