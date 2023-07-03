#pragma once

#include "Hudi/Core/UUID.h"
#include "Component.h"

namespace Hudi {

	class IDComponent
	{
	public:
		IDComponent() : ID(UUID()) {}
		IDComponent(const UUID& ID) : ID(ID) {}
		virtual ~IDComponent() = default;

		UUID ID;
	};
}