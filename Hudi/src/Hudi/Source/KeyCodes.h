#pragma once


namespace Hudi {

	//Source: https://github.com/TheCherno/Hazel/blob/master/Hazel-ScriptCore/Source/Hazel/KeyCode.cs

	using KeyCode = int;
	
	namespace Key
	{
		enum : KeyCode
		{
			SPACE = 32, //SDLK_SPACE,

			D0 = 48, //SDLK_0
			D1 = 49, //SDLK_1,
			D2 = 50, //SDLK_2,
			D3 = 51, //SDLK_3,
			D4 = 52, //SDLK_4,
			D5 = 53, //SDLK_5,
			D6 = 54, //SDLK_6,
			D7 = 55, //SDLK_7,
			D8 = 56, //SDLK_8,
			D9 = 57, //SDLK_9,

			A = (char)'a', //SDLK_a,
			B = (char)'b', //SDLK_b,
			C = (char)'c', //SDLK_c,
			D = (char)'d', //SDLK_d,
			E = (char)'e', //SDLK_e,
			F = (char)'f', //SDLK_f,
			G = (char)'g', //SDLK_g,
			H = (char)'h', //SDLK_h,
			I = (char)'i', //SDLK_i,
			J = (char)'j', //SDLK_j,
			K = (char)'k', //SDLK_k,
			L = (char)'l', //SDLK_l,
			M = (char)'m', //SDLK_m,
			N = (char)'n', //SDLK_n,
			O = (char)'o', //SDLK_o,
			P = (char)'p', //SDLK_p,
			Q = (char)'q', //SDLK_q,
			R = (char)'r', //SDLK_r,
			S = (char)'s', //SDLK_s,
			T = (char)'t', //SDLK_t,
			U = (char)'u', //SDLK_u,
			V = (char)'v', //SDLK_v,
			W = (char)'w', //SDLK_w,
			X = (char)'x', //SDLK_x,
			Y = (char)'y', //SDLK_y,
			Z = (char)'z', //SDLK_z,

			/* Function keys */
			L_CTRL = 1073742048, //SDLK_LCTRL,
			R_CTRL = 1073742052, //SDLK_RCTRL,
			L_SHIFT = 1073742049, //SDLK_LSHIFT,
			R_SHIFT = 1073742053, //SDLK_RSHIFT,
			L_ALT = 1073742050, //SDLK_LALT,
			R_ALT = 1073742054, //SDLK_RALT
			ENTER = 13, //SDLK_RETURN,

			RIGHT	= 79 | (1 << 30), //SDLK_RIGHT,
			LEFT	= 80 | (1 << 30), //SDLK_LEFT,
			DOWN	= 81 | (1 << 30), //SDLK_DOWN,
			UP		= 82 | (1 << 30) //SDLK_UP
		};
		
	};

}