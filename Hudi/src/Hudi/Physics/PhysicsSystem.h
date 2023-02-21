#pragma once

#include <ECS.h>

#include "CollisionDetector.h"

namespace Hudi {

	class PhysicsSystem : public ECS::System
	{
	public:
		PhysicsSystem();

		void OnUpdate() override;

	private:
		CollisionDetector m_CollisionDetector;
	};


}