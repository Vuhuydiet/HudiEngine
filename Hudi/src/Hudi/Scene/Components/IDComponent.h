#pragma once

#include "Hudi/Core/UUID.h"
#include "Component.h"

namespace Hudi {

	class IDComponent
	{
	public:
		IDComponent();
		IDComponent(UUID ID);

		UUID ID;
	};

}