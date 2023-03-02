#pragma once

#include "Hudi/Core/Layer.h"

namespace Hudi {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnEvent(Event& event);
		void OnUpdate();
	};

}