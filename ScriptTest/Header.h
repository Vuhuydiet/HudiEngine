#pragma once

#ifdef SCRIPTTEST_EXPORTS
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __declspec(dllimport
#endif

#include <HudiAPI/Interface.h>
#include <Hudi/Core/Input.h>
#include <Hudi.h>

class SCRIPT_API Movement : public hd_api::ScriptComponent
{
public:
	void Awake() override
	{
	}
	void Update(float dt) override
	{
		auto& transform = GetComponent<Hudi::Transform>();
		//transform.rotation.z += 60.0f * dt;
		//Hudi::Print();
		//std::cout << 1;
		auto pos = Hudi::Input::GetMousePosition();
		printf("[%f, %f]\n", pos.x, pos.y);
		if (Hudi::Input::IsKeyDown(Hudi::Key::D)) {
			transform.position.x += v * dt;
		}
	}

	float v = 1.0f;
};
