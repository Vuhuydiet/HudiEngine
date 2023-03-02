#include <Hudi.h>

Hudi::GameObject& CreateBackground(Hudi::Scene& scene)
{
	Hudi::GameObject& bg = scene.CreateGameObject("background");

	auto& transform = bg.GetComponent<Hudi::Transform>();
	transform.scale.x = 2.0f;
	transform.scale.y = 2.0f;

	auto& sprite = bg.GetComponent<Hudi::SpriteRenderer>();
	sprite.order = 0;
	sprite.SetFile("assets/IndustrialZoneTileSet/Background/Background.png");

	return bg;
}