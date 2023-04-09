#include "hdpch.h"
#include "Time.h"

#include <SDL.h>

namespace Hudi {

//	uint32_t Time::s_FPS = 60;
//	float Time::s_dt = (float) 1.0f / s_FPS;
//
//	void Time::Delay(float duration)
//	{
//		uint32_t dur = static_cast<uint32_t>(duration * 1000);
//		SDL_Delay(dur);
//	}
//
//	uint32_t Time::FPS() 
//	{ 
//		return s_FPS;
//	}
//	float Time::deltaTime()
//	{ 
//		return s_dt; 
//	}
//
//	void Time::SetFPS(uint32_t fps)
//	{ 
//		s_FPS = fps, s_dt = 1.0f / fps; 
//	}
//
//
//	Timer::Timer()
//	{
//		Reset();
//	}
//
//	Timer::Timer(float duration)
//		: m_Duration(uint32_t(duration * 1000))
//	{
//		Reset();
//	}
//
//	Timer::~Timer()
//	{
//		uint32_t elapsed = Elapsed();
//
//		if (elapsed < m_Duration)
//		{
//			Time::Delay(static_cast<float>((m_Duration - elapsed) / 1000.0f));
//		}
//	}
//
//	void Timer::Reset()
//	{
//		m_Start = 0;//SDL_GetTicks();
//	}
//
//	uint32_t Timer::Elapsed() 
//	{
//		return 0; // SDL_GetTicks() - m_Start;
//	}

}