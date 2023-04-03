#pragma once
#include "Core.h"

#include "Log.h"

#ifdef HD_PLATFORM_WINDOWS

extern Hudi::Application* Hudi::CreateApplication();

int main(int argc, char** argv)
{
	Hudi::Log::Init();
	HD_CORE_INFO("Initialised Log in EntryPoint.h!");
		
	Hudi::Application* app = Hudi::CreateApplication();
	app->Init();
	HD_CORE_INFO("Initialised App in EntryPoint.h!");

	app->Run();

	delete app;
	HD_CORE_INFO("Program Closed in EntryPoint.h!");

	return 0;
}

#endif
