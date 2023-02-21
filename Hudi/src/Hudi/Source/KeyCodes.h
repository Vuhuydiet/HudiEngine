#pragma once
#include <SDL.h>

namespace Hudi {

	//Source: https://github.com/TheCherno/Hazel/blob/master/Hazel-ScriptCore/Source/Hazel/KeyCode.cs

	using KeyCode = int;
	
	namespace Key
	{
		enum : KeyCode
		{
			SPACE = SDLK_SPACE,

			D0 = SDLK_0,
			D1 = SDLK_1,
			D2 = SDLK_2,
			D3 = SDLK_3,
			D4 = SDLK_4,
			D5 = SDLK_5,
			D6 = SDLK_6,
			D7 = SDLK_7,
			D8 = SDLK_8,
			D9 = SDLK_9,

			A = SDLK_a,
			B = SDLK_b,
			C = SDLK_c,
			D = SDLK_d,
			E = SDLK_e,
			F = SDLK_f,
			G = SDLK_g,
			H = SDLK_h,
			I = SDLK_i,
			J = SDLK_j,
			K = SDLK_k,
			L = SDLK_l,
			M = SDLK_m,
			N = SDLK_n,
			O = SDLK_o,
			P = SDLK_p,
			Q = SDLK_q,
			R = SDLK_r,
			S = SDLK_s,
			T = SDLK_t,
			U = SDLK_u,
			V = SDLK_v,
			W = SDLK_w,
			X = SDLK_x,
			Y = SDLK_y,
			Z = SDLK_z,

			/* Function keys */
			ENTER = SDLK_RETURN,

			RIGHT = SDLK_RIGHT,
			LEFT = SDLK_LEFT,
			DOWN = SDLK_DOWN,
			UP = SDLK_UP
		};
		
	};

}