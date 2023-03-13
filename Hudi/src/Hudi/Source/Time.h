#pragma once
#include "hdpch.h"
#include "Hudi/Core/Core.h"

#include <SDL.h>

namespace Hudi {

	class HUDI_API Time
	{
	public:

		static void Delay(float duration)
		{
			Uint32 dur = static_cast<Uint32>(duration * 1000);
			SDL_Delay(dur);
		}

		static uint32_t FPS() { return s_FPS; }
		static float deltaTime() { return s_dt; }

		static void SetFPS(uint32_t fps) { s_FPS = fps, s_dt = 1.0f / fps; }

	private:
		static uint32_t s_FPS;
		static float s_dt; // unit: s
	};


	class HUDI_API Timer
	{
	public:
		Timer()
		{
			Reset(); 
		}
		Timer(float duration) 
			: m_Duration( Uint32(duration * 1000) )
		{
			Reset();
		}

		~Timer() { 
			Uint32 elapsed = Elapsed();

			if (elapsed < m_Duration)
			{
				Time::Delay(static_cast<float>((m_Duration - elapsed)/1000.0f));
			}
		}

		void Reset() { m_Start = SDL_GetTicks(); }
		Uint32 Elapsed() { return SDL_GetTicks() - m_Start; }

	private:
		Uint32 m_Start; // unit: ms
		Uint32 m_Duration;
	};

}