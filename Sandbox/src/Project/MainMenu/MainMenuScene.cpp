#pragma once
#include <Hudi.h>

Hudi::GameObject& CreateController(Hudi::Scene & scene);
Hudi::GameObject& CreateMainCamera(Hudi::Scene & scene);
Hudi::GameObject& CreatePlayer1(Hudi::Scene& scene);
Hudi::GameObject& CreatePlayer2(Hudi::Scene& scene);
Hudi::GameObject& CreateBackground(Hudi::Scene& scene);
Hudi::GameObject& CreateGround(Hudi::Scene& scene);
#include "Controller.h"
#include "CameraMovement.h"
void MainMenuScene(Hudi::Scene& scene)
{
	auto& controller = CreateController(scene);

	auto& cam1 = CreateMainCamera(scene);
	auto& cam2 = CreateMainCamera(scene);
	auto& player1 = CreatePlayer1(scene);
	auto& player2 = CreatePlayer2(scene);
	auto& background = CreateBackground(scene);
	auto& ground = CreateGround(scene);

	controller.GetComponent<Controller>().cams.push_back(&cam1);
	controller.GetComponent<Controller>().cams.push_back(&cam2);

	cam1.GetComponent<CameraMovement>().player_transform = &player1.GetComponent<Hudi::Transform>();
	cam2.GetComponent<CameraMovement>().player_transform = &player2.GetComponent<Hudi::Transform>();
	scene.SetActiveCamera(cam1);
}