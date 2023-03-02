#include <Hudi.h>

#include "Controller.h"
Hudi::GameObject& CreateController(Hudi::Scene& scene)
{
	auto& controller = scene.CreateGameObject("Controller");
	auto& script = controller.AddComponent<Controller>();
	return controller;
}