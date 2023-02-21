#include "ecspch.h"
#include "Initializer.h"

namespace ECS {

	std::unique_ptr<EntityManager> Coordinator::s_EntityManager = nullptr;
	std::unique_ptr<ComponentManager> Coordinator::s_ComponentManager = nullptr;
	std::unique_ptr<SystemManager> Coordinator::s_SystemManager = nullptr;
	std::queue<Entity> Coordinator::s_DestroyedEntities;

}