#include "Hudi.h"

void CreateGround(Hudi::Scene& scene)
{
	Hudi::GameObject* ground[5];

	float scale = 2.0f;

	int y = 500;

	for (int i = 0; i < 5; i++)
	{
		ground[i] = &scene.CreateGameObject("ground");
		auto& g = *ground[i];

		auto& trans = g.GetComponent<Hudi::Transform>();
		trans.position.x = 128 * i * scale;
		trans.position.y = static_cast<float>(y);
		trans.scale *= scale;

		auto& sprite = g.GetComponent<Hudi::SpriteRenderer>();
		sprite.order = 1;
		sprite.SetFile("assets/IndustrialZoneTileSet/Animated objects/Transporter2.png");

		g.AddComponent<Hudi::RigidBody2D>();
		auto& rigid = g.GetComponent<Hudi::RigidBody2D>();

		g.AddComponent<Hudi::BoxCollider2D>();
		auto& box = g.GetComponent<Hudi::BoxCollider2D>();
		box.tag = "ground";
		box.minPoint = { trans.position.x, trans.position.y };
		box.maxPoint = { trans.position.x + 128 * scale, trans.position.y + 48 / 2 * scale };
	}
}