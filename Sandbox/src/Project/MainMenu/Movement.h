#include <HudiEngine.h>

class Movement : public MonoBehaviour
{
public:

	void Awake()
	{
		trans = &GetComponent<Transform>();
		body = &GetComponent<RigidBody2D>();
		anim = &GetComponent<Animator>();
		ani = &GetComponent<Animation>();
		sprite = &GetComponent<SpriteRenderer>();
		box = &GetComponent<BoxCollider2D>();
	}

	void Update()
	{
		Move();
		Jump();
		Slow(0.1f);
		int rate = 1;
		if (Input::IsKeyDown(Key::D1))
		{
			box->minPoint.x -= rate;
		}
		if (Input::IsKeyDown(Key::D2))
		{
			box->minPoint.x += rate;
		}
		if (Input::IsKeyDown(Key::D3))
		{
			box->minPoint.y -= rate;
		}
		if (Input::IsKeyDown(Key::D4))
		{
			box->minPoint.y += rate;
		}
		if (Input::IsKeyDown(Key::D5))
		{
			box->maxPoint.x -= rate;
		}
		if (Input::IsKeyDown(Key::D6))
		{
			box->maxPoint.x += rate;
		}

	}

	void OnCollisionEnter2D(Collision2D collision)
	{
		if (collision.GetOtherEntityComponent<BoxCollider2D>().tag == "ground")
			grounded = true;
	}

	void Move()
	{
		if (Input::IsKeyDown(Key::A))
		{
			body->velocity.x = -300;
			trans->scale.x = -abs(trans->scale.x);
		}
		if (Input::IsKeyDown(Key::D))
		{
			body->velocity.x = 300;
			trans->scale.x = abs(trans->scale.x);
		}

		if (Input::IsKeyUp(Key::A) && Input::IsKeyUp(Key::D))
			body->velocity.x = 0;

		anim->SetBool("isRunning", body->velocity.x != 0);
	}

	void Jump()
	{
		if (Input::IsKeyPressed(Key::W) && grounded)
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

	void Print()
	{
		HD_INFO("{0}", 0);
	}

private:
	bool grounded = false;


	Transform* trans;
	RigidBody2D* body;
	Animator* anim;
	Animation* ani;
	SpriteRenderer* sprite;
	BoxCollider2D* box;
};
