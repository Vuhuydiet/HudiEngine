#pragma once

#include "Hudi/Core/Layer.h"
#include "Hudi/Core/Window.h"

namespace Hudi {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();
	};

}