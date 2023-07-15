#pragma once

#ifdef SCRIPTTEST_EXPORTS
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __declspec(dllimport
#endif

#include <HudiAPI/HudiAPI.h>
#include <Hudi.h>

using namespace Hudi;
using namespace hd_api;

class SCRIPT_API Movement : public Behaviour
{
public:
	void Awake() override
	{
	}

	void Update(float dt)
	{
		auto& transform = GetComponent<Transform>();
		glm::vec3 dir = { 0, 0, 0 };
		if (Input::IsKeyDown(Key::D)) {
			dir.x += 1;
		}
		if (Input::IsKeyDown(Key::A)) {
			dir.x -= 1;
		}
		if (Input::IsKeyDown(Key::W)) {
			dir.y += 1;
		}
		if (Input::IsKeyDown(Key::S)) {
			dir.y -= 1;
		}
		transform.position += dir * v * dt;
	}

	SERIALIZE_FIELD float v = 1.0f;
};

class SCRIPT_API Attack : public hd_api::Behaviour
{
public:
	void Update(float dt) override {
		if (Input::IsKeyPressed(Key::F)) {
			printf("Shooting!\n");
		}
	}
};
