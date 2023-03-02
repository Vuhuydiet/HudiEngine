#include <Hudi.h>

#include "CameraMovement.h"
Hudi::GameObject& CreateMainCamera(Hudi::Scene& scene)
{
	auto& cam = scene.CreateGameObject("Main camera");

	cam.AddComponent<CameraMovement>();

	auto& transform = cam.GetComponent<Hudi::Transform>();
	transform.position.z = 10;

	auto& c = cam.AddComponent<Hudi::Camera>();
	c.type = Hudi::Camera::Perspective;
	
	c.C = 20;
	return cam;
}