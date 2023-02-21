#include <HudiEngine.h>

class Movement2 : public MonoBehaviour
{
public:

	void Awake()
	{
		trans = &GetComponent<Transform>();
		body = &GetComponent<RigidBody2D>();
		anim = &GetComponent<Animator>();
		ani = &GetComponent<Animation>();
		sprite = &GetComponent<SpriteRenderer>();
	}

	void Update()
	{
		Move();
		Jump();
		Slow(0.1f);
	}

	void OnCollisionEnter2D(Collision2D collision)
	{
		if (collision.GetOtherEntityComponent<BoxCollider2D>().tag == "ground")
			grounded = true;
	}

	void Move()
	{
		if (Input::IsKeyDown(Key::LEFT))
		{
			body->velocity.x = -300;;
			trans->scale.x = -abs(trans->scale.x);
		}
		if (Input::IsKeyDown(Key::RIGHT))
		{
			body->velocity.x = 300;
			trans->scale.x = abs(trans->scale.x);
		}

		if (Input::IsKeyUp(Key::LEFT) && Input::IsKeyUp(Key::RIGHT))
			body->velocity.x = 0;

		anim->SetBool("isRunning", body->velocity.x != 0);
	}

	void Jump()
	{
		if (Input::IsKeyPressed(Key::UP) && grounded)
		{
			grounded = false;
			body->velocity.y = -1000;
		}
	}

	void Slow(float _speed)
	{
		static bool flag = false;
		if (Input::IsKeyPressed(Key::F) && !flag)
		{
			body->velocity *= _speed;
			ani->speed = _speed;
			flag = true;
		}
		else
		{
			flag = false;
			ani->speed = 1.0f;
		}
	}

private:
	bool grounded = false;


	Transform* trans;
	RigidBody2D* body;
	Animator* anim;
	Animation* ani;
	SpriteRenderer* sprite;
};
