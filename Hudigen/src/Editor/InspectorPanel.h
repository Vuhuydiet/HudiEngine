#pragma once

#include <Hudi.h>

namespace Hudi {

	class InspectorPanel
	{
	public:
		void OnImGuiRender(bool& open, GameObject object);
	private:
		void OnDrawComponents(GameObject object);
		void OnDrawAddComponent(GameObject object);
	};

}