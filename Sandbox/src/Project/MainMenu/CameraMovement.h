#pragma once
#include <HudiEngine.h>

class CameraMovement : public MonoBehaviour
{
public:
	void Awake()
	{
		trans = &GetComponent<Transform>();
	}

	void Update()
	{
		trans->position.x += 0.08f * (player_transform->position.x - trans->position.x);
		trans->position.y += 0.08f * (player_transform->position.y - trans->position.y);

		if (Input::IsKeyDown(Key::F))
			trans->position.z -= 1;
		if (Input::IsKeyDown(Key::G))
			trans->position.z += 1;
	}
	const Transform* player_transform = nullptr;
private:
	Transform* trans = nullptr;
};