#pragma once

#include "Hudi/Core/Core.h"

#include "Hudi/Source/KeyCodes.h"
#include "Hudi/Source/MouseCodes.h"

#include "Hudi/Source/Vec2.h"

namespace Hudi {

	class HUDI_API Input
	{
	public:
		static bool Quit();

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);
		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool IsMousePressed(MouseCode buttoncode);
		static bool IsMouseReleased(MouseCode buttoncode);
		static bool IsMouseDown(MouseCode button);
		static bool IsMouseUp(MouseCode button);

		static Vec2<int> GetMousePosition();
	};

}
