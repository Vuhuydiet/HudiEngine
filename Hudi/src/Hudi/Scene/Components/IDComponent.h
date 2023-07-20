#pragma once

#include "Hudi/Core/UUID.h"

namespace Hudi {

	class IDComponent
	{
	public:
		IDComponent();
		IDComponent(UUID ID);

		UUID ID;
	};

}