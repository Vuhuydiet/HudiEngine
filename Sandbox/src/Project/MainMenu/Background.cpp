#include <Hudi.h>

void CreateBackground(Hudi::Scene& scene)
{
	Hudi::GameObject& bg = scene.CreateGameObject("background");

	auto& trans = bg.GetComponent<Hudi::Transform>();
	trans.scale.x = 2.0f;
	trans.scale.y = 2.0f;

	auto& sprite = bg.GetComponent<Hudi::SpriteRenderer>();
	sprite.order = 0;
	sprite.SetFile("assets/IndustrialZoneTileSet/Background/Background.png");
}