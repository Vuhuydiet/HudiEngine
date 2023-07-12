#include "Components.h"

extern "C" SCRIPT_API void SetInputSource(Hudi::EventManager* instance)
{
	Hudi::EventManager::SetInstance(instance);
}

extern "C" SCRIPT_API hd_api::Behaviour* InstantiateScript1(Hudi::GameObject object)
{
	if (!object.IsValid())
		return nullptr;
	return &object.AddComponent<Movement>();
}

extern "C" SCRIPT_API hd_api::Behaviour* InstantiateScript2(Hudi::GameObject object)
{
	if (!object.IsValid())
		return nullptr;
	return &object.AddComponent<Attack>();
}