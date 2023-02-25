#include <Hudi.h>

#include "Movement2.h"

void CreatePlayer2(Hudi::Scene& scene)
{
	Hudi::GameObject& p1 = scene.CreateGameObject("Player2");
	// Transform
	auto& trans = p1.GetComponent<Hudi::Transform>();
	trans.position.x = 700;
	trans.position.y = 300;
	trans.scale.x = 3.0f;
	trans.scale.y = 3.0f;

	// Sprite Renderer
	auto& sprite = p1.GetComponent<Hudi::SpriteRenderer>();
	sprite.order = 1;

	// RigidBody2D
	auto& body = p1.AddComponent<Hudi::RigidBody2D>();
	body.bodyType = Hudi::RigidBody2D::BodyType::Dynamic;
	body.gravityScale = 3000.0f;

	// BoxCollider
	auto& box = p1.AddComponent<Hudi::BoxCollider2D>();
	box.minPoint = { trans.position.x + 10, trans.position.y + 40 };
	box.maxPoint = { trans.position.x + 60, trans.position.y + 144 };
	box.isDrawn = true;

	// Script
	p1.AddComponent<Movement2>();


	// Animations
	Hudi::Animation idle("Idle");
	idle.filePath = "assets/CyberPunk/Knife/Characters/3 Cyborg/Idle.png";
	idle.SetFrameTimes(std::vector<float>(4, 0.1f));
	idle.SetFrameSize(48, 48);

	Hudi::Animation run("Run");
	run.filePath = "assets/CyberPunk/Knife/Characters/3 Cyborg/Run.png";
	run.SetFrameTimes(std::vector<float>(6, 0.1f));
	run.SetFrameSize(48, 48);

	// Animator
	auto& animator = p1.AddComponent<Hudi::Animator>();

	animator.Add_Bool_Parameter("isRunning");

	animator.AddAnimation(idle);
	animator.AddAnimation(run);

	animator.MakeTransition("Idle", "Run");
	animator.MakeTransition("Run", "Idle");
	animator.GetTransition("Idle", "Run").AddCondition("isRunning", true);
	animator.GetTransition("Run", "Idle").AddCondition("isRunning", false);
}