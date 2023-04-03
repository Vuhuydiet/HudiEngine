#pragma once

#include "hdpch.h"

namespace Hudi {

	class Clock
	{
	public:
		Clock()
			: m_Start(std::chrono::high_resolution_clock::now())
		{}

		float GetSeconds()
		{
			auto now = std::chrono::high_resolution_clock::now();
			long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_Start).count();
			return duration * 1.0f / 1000.0f;
		}
		float GetMilliseconds()
		{
			auto now = std::chrono::high_resolution_clock::now();
			long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_Start).count();
			return (float)duration;
		}

	private:
		std::chrono::steady_clock::time_point m_Start;
	};

}