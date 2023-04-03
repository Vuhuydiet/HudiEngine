#pragma once
#include "hdpch.h"
#include "Hudi/Events/Event.h"

namespace Hudi {

	class Layer
	{
	public:
		Layer(const std::string& _name = "Layer") 
			: m_DebugName(_name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}
		// unit: seconds
		virtual void OnUpdate(float dt) {}

		virtual void OnImGuiRender() {}

	private:
		std::string m_DebugName;
	};

}