#pragma once
#include "HudiEngine.h"


class Controller : public MonoBehaviour
{
public:
	void Awake()
	{

	}
	void Update()
	{
		if (Input::IsKeyPressed(Key::C))
		{
			cur++;
			if (cur >= cams.size())
				cur = 0;
			Hudi::SceneManager::GetActiveScene().SetActiveCamera(*cams[cur]);
		}
	}

	std::vector<GameObject*> cams;
	int cur = 0;
};