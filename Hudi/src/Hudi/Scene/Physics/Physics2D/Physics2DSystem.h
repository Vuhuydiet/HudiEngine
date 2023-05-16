#pragma once

#include <ECS.h>

class b2World;

namespace Hudi {

	class Physics2DSystem : public ECS::System 
	{
	public:
		Physics2DSystem();

		virtual void OnEntityAdded(ECS::Entity entity) override;
		virtual void OnEntityRemoved(ECS::Entity entity) override;

		void Begin();
		void End();
		void OnUpdate(float dt);
		
	private:
		void AddEntityPhysicsDef(ECS::Entity entity);
		void RemoveEntityPhyicsDef(ECS::Entity entity);
	private:
		b2World* m_PhysicsWorld = nullptr;

		bool m_OnRuntime = false;
	};

}