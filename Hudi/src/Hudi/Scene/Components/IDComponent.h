#pragma once

#include "Hudi/Core/UUID.h"
#include "Component.h"

namespace Hudi {

	class IDComponent : public Component
	{
	public:
		IDComponent() : ID(UUID()) {}
		IDComponent(const UUID& ID) : ID(ID) {}
		virtual const char* ToString() const override { return "UUID"; }

		UUID ID;
	};
}