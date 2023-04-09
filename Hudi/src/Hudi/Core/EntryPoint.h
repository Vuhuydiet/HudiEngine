#pragma once

#include "Core.h"
#include "Log.h"
#include "Application.h"
#include "Hudi/Debug/Instrumentor.h"

#ifdef HD_PLATFORM_WINDOWS

extern Hudi::Application* Hudi::CreateApplication();

int main(int argc, char** argv)
{
	Hudi::Log::Init();

	HD_PROFILE_BEGIN_SESSION("Startup", "HudiProfile-Startup.json");
	Hudi::Application* app = Hudi::CreateApplication();
	app->Init();
	HD_PROFILE_END_SESSION();

	HD_PROFILE_BEGIN_SESSION("Runtime", "HudiProfile-Runtime.json");
	app->Run();
	HD_PROFILE_END_SESSION();

	HD_PROFILE_BEGIN_SESSION("Shutdown", "HudiProfile-Shutdown.json");
	delete app;
	HD_PROFILE_END_SESSION();

	return 0;
}

#endif
