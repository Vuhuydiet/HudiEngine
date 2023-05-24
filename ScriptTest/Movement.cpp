#include "pch.h"

#include <Hudi/Scene/Components/Component.h>
#include <Hudi/Core/Input.h>

#include <iostream>

class MOV_API Movement : public hd_api::ScriptComponent
{
public:
	void Awake() override
	{
	}
	void Update(float dt) override
	{
		auto& transform = GetComponent<Hudi::Transform>();
		transform.rotation.z += 60.0f * dt;
		if (Hudi::Input::IsKeyDown(Hudi::Key::D))
		{
			transform.position.x += v * dt;
			std::cout << v * dt << "\n";
		}
		if (Hudi::Input::IsKeyDown(Hudi::Key::A))
		{
			std::cout << v * dt << "\n";
			transform.position.x -= v * dt;
		}
	}

	float v = 1.0f;
};

extern "C" MOV_API hd_api::ScriptComponent* InstantiateScript()
{
	return new Movement();
}