#include <Hudi.h>

#include "Movement2.h"

void CreatePlayer2(Hudi::Scene& scene)
{
	Hudi::GameObject& p1 = scene.CreateGameObject("Player1");
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
	p1.AddComponent<Hudi::RigidBody2D>();
	auto& rigid = p1.GetComponent<Hudi::RigidBody2D>();
	rigid.bodyType = Hudi::RigidBody2D::BodyType::Dynamic;
	rigid.gravityScale = 3000.0f;

	// BoxCollider
	p1.AddComponent<Hudi::BoxCollider2D>();
	auto& box = p1.GetComponent<Hudi::BoxCollider2D>();
	box.minPoint = { trans.position.x + 25, trans.position.y + 25 };
	box.maxPoint = { trans.position.x + 48 * trans.scale.x - 25, trans.position.y + 48 * trans.scale.y };
	box.isDrawn = true;

	// Script
	p1.AddComponent<Movement2>();


	// Animations
	Hudi::Animation idle("Idle");
	idle.filePath = "assets/CyberPunk/Knife/Characters/Cyborg/Idle.png";
	idle.SetFrameTimes(std::vector<float>(4, 0.1f));
	idle.SetFrameSize(48, 48);

	Hudi::Animation run("Run");
	run.filePath = "assets/CyberPunk/Knife/Characters/Cyborg/Run.png";
	run.SetFrameTimes(std::vector<float>(6, 0.1f));
	run.SetFrameSize(48, 48);

	// Animator
	p1.AddComponent<Hudi::Animator>();
	auto& animator = p1.GetComponent<Hudi::Animator>();

	animator.Add_Bool_Parameter("isRunning");

	animator.AddAnimation(idle);
	animator.AddAnimation(run);

	animator.MakeTransition("Idle", "Run");
	animator.MakeTransition("Run", "Idle");
	animator.GetTransition("Idle", "Run").AddCondition("isRunning", true);
	animator.GetTransition("Run", "Idle").AddCondition("isRunning", false);
}