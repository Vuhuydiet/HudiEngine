#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>

#include "EditorLayer.h"

#include <SDL.h>

namespace Hudi {

	class HudiGen : public Application
	{
	public:
		HudiGen()
			: Application(WindowProps("HudiGen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI))
		{
			HD_WARN("Currently disable FPS controller!");
			//Hudi::Time::SetFPS(30);

			PushLayer(new EditorLayer());

		}

	private:

	};

}

Hudi::Application* Hudi::CreateApplication()
{
	return new HudiGen();
}
