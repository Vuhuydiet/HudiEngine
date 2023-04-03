#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace Hudi {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define HD_CORE_TRACE(...)		::Hudi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HD_CORE_INFO(...)		::Hudi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HD_CORE_WARN(...)		::Hudi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HD_CORE_ERROR(...)		::Hudi::Log::GetCoreLogger()->error(__VA_ARGS__)

// Log macros
#define HD_TRACE(...)			::Hudi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HD_INFO(...)			::Hudi::Log::GetClientLogger()->info(__VA_ARGS__)
#define HD_WARN(...)			::Hudi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HD_ERROR(...)			::Hudi::Log::GetClientLogger()->error(__VA_ARGS__)