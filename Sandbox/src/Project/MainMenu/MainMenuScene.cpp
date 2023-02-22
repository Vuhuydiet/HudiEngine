#pragma once
#include <Hudi.h>

void CreatePlayer1(Hudi::Scene& scene);
void CreatePlayer2(Hudi::Scene& scene);
void CreateBackground(Hudi::Scene& scene);
void CreateGround(Hudi::Scene& scene);

void MainMenuScene(Hudi::Scene& scene)
{
	CreatePlayer1(scene);
	CreatePlayer2(scene);
	CreateBackground(scene);
	CreateGround(scene);
}