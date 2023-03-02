#include "hdpch.h"
#include "Time.h"

namespace Hudi {

	uint32_t Time::s_FPS = 60;
	float Time::s_dt = (float) 1.0f / s_FPS;

}