#pragma once

namespace Hudi {

	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			BUTTON_LEFT		= 1, // SDL_BUTTON_LEFT,
			BUTTON_MIDDLE	= 2, // SDL_BUTTON_MIDDLE,
			BUTTON_RIGHT	= 3, // SDL_BUTTON_RIGHT,
			BUTTON_X1		= 4, // SDL_BUTTON_X1,
			BUTTON_X2		= 5 // SDL_BUTTON_X2
		};
	}

	/*enum MouseButtonCode
	{
		Button_LEFT			= SDL_BUTTON_LEFT,
		Button_MIDDLE		= SDL_BUTTON_MIDDLE,
		Button_RIGHT		= SDL_BUTTON_RIGHT,
		Button_X1			= SDL_BUTTON_X1,
		Button_X2			= SDL_BUTTON_X2
	};*/

}