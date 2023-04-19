#pragma once

#include <Hudi.h>

namespace Hudi {

	class MenuBar
	{
	public:
		Ref<Scene> OnImGuiRender(Ref<Scene> scene);

		Ref<Scene> NewScene();
		Ref<Scene> OpenScene();
		void SaveSceneAs(Ref<Scene> scene);
	
	public:
		bool opt_Hierarchy = true;
		bool opt_Inspector = true;
	};

}