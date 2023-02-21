#pragma once
#include "Core.h"

#ifdef HD_PLATFORM_WINDOWS

extern Hudi::Application* Hudi::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hudi::CreateApplication();
	app->Init();
	HD_CORE_INFO("Initialised App in EntryPoint.h !");

	app->Run();

	delete app;
	HD_CORE_INFO("Program Closed in EntryPoint.h !");

	return 0;
}

#endif
