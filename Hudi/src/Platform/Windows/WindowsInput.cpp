#include "hdpch.h"
#include "Hudi/Core/Input.h"

#include "Hudi/Events/Event.h"

namespace Hudi {

	bool Input::Quit()
	{
		return EventManager::Quit();
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		return EventManager::KeyJustDown(keycode);
	}

	bool Input::IsKeyReleased(KeyCode keycode)
	{
		return EventManager::KeyJustUp(keycode);
	}

	bool Input::IsKeyDown(KeyCode key)
	{
		return EventManager::KeyDown(key);
	}

	bool Input::IsKeyUp(KeyCode key)
	{
		return EventManager::KeyUp(key);
	}

	bool Input::IsMousePressed(MouseCode buttoncode)
	{
		return EventManager::MouseJustDown(buttoncode);
	}

	bool Input::IsMouseReleased(MouseCode buttoncode)
	{
		return EventManager::MouseJustUp(buttoncode);
	}

	bool Input::IsMouseDown(MouseCode button)
	{
		return EventManager::MouseDown(button);
	}

	bool Input::IsMouseUp(MouseCode button)
	{
		return EventManager::MouseUp(button);
	}

	Vec2<int> Input::GetMousePosition()
	{
		return EventManager::MousePosition();
	}

}