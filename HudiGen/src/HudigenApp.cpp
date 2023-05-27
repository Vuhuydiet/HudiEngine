#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hudi {

	class HudiGen : public Application
	{
	public:
		HudiGen()
			: Application(WindowProps("HudiGen", WINDOWPOS_CENTERED, WINDOWPOS_CENTERED,
				1280, 720, WINDOW_OPENGL | WINDOW_RESIZABLE | WINDOW_ALLOW_HIGHDPI))
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
