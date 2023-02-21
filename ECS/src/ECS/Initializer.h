#pragma once

#include "ecspch.h"

#include "Coordinator.h"

namespace ECS {

	void Init_Everything();

	void Destroy_Everything();



	inline void Init_Everything()
	{
		Coordinator::Init();
	}

	inline void Destroy_Everything()
	{
	}
}