#include <Hudi.h>
#include "Movement.h"
Hudi::GameObject& CreatePlayer1(Scene& scene)
{
	auto& go = scene.CreateGameObject("Player1");
	go.AddComponent<Movement>();
	auto& transform = go.GetComponent<Transform>();
		transform.position.x = 300;
		transform.position.y = 300;
		transform.position.z = 0;
		transform.rotation.x = 0;
		transform.rotation.y = 0;
		transform.rotation.z = 0;
		transform.scale.x = 3.0f;
		transform.scale.y = 3.0f;
		transform.scale.z = 0;
	auto& sprite = go.GetComponent<SpriteRenderer>();;
		sprite.order = 1;
	auto& body = go.AddComponent<RigidBody2D>();
		body.bodyType = RigidBody2D::BodyType::Dynamic;
		body.gravityScale = 3000.0f;
		body.SetMass(1.f);
	auto& box = go.AddComponent<BoxCollider2D>();
		box.tag = "";
		box.minPoint.x = transform.position.x + 10;
		box.minPoint.y = transform.position.y + 40;
		box.maxPoint.x = transform.position.x + 60;
		box.maxPoint.y = transform.position.y + 144;
		box.isDrawn = true;
	Animation Idle("Idle");
		Idle.filePath = "assets/CyberPunk/Knife/Characters/2 Punk/Idle.png";
		Idle.SetFrameTimes( { 0.1f, 0.1f, 0.1f, 0.1f } );
		Idle.SetFrameSize(48, 48);
	Animation Run("Run");
		Run.filePath = "assets/CyberPunk/Knife/Characters/2 Punk/Run.png";
		Run.SetFrameTimes( { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f } );
		Run.SetFrameSize(48, 48);
	auto& anim = go.AddComponent<Animator>();
		anim.Add_Bool_Parameter("isRunning");
		anim.AddAnimation(Idle);
		anim.AddAnimation(Run);
		anim.MakeTransition("Idle", "Run");
		anim.MakeTransition("Run", "Idle");
		anim.GetTransition("Idle", "Run").AddCondition("isRunning", true);
		anim.GetTransition("Run", "Idle").AddCondition("isRunning", false);

		return go;
}
