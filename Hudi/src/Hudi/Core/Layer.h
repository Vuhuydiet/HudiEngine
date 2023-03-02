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
		virtual void OnUpdate() {}

	private:
		std::string m_DebugName;
	};

}