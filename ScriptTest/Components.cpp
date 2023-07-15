#include "Components.h"

extern "C" {

	SCRIPT_API void SetInputSource(Hudi::EventManager* instance) { Hudi::EventManager::SetInstance(instance); }

	SCRIPT_API void GetBehaviourComponentNames(const char** buffers, size_t* size)
	{
		*size = 2;
		buffers[0] = "Movement";
		buffers[1] = "Attack";
	}

	SCRIPT_API hd_api::Behaviour* InstantiateMovement(Hudi::GameObject object)
	{
		if (!object.IsValid())
			return nullptr;
		return &object.AddComponent<Movement>();
	}
	SCRIPT_API void DestroyMovement(Hudi::GameObject object)
	{
		object.RemoveComponent<Movement>();
	}

	SCRIPT_API hd_api::Behaviour* InstantiateAttack(Hudi::GameObject object)
	{
		if (!object.IsValid())
			return nullptr;
		return &object.AddComponent<Attack>();
	}
	SCRIPT_API void DestroyAttack(Hudi::GameObject object)
	{
		object.RemoveComponent<Attack>();
	}

}
