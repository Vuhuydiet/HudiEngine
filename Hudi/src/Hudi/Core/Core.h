#pragma once

#ifdef HD_PLATFORM_WINDOWS
	#ifdef HD_DYNAMIC_LINK
		#ifdef HD_BUILD_DLL
			#define HUDI_API __declspec(dllexport)
		#else
			#define HUDI_API __declspec(dllimport)
		#endif
	#else
		#define HUDI_API
	#endif
#else
	#ifdef HD_PLATFORM_MACOS
		#define HUDI_API
	#else
		#error Hudi only supports Windows!
	#endif
#endif

#ifdef HD_DEBUG
	#define HD_ENABLE_ASSERTS
#endif

#ifdef HD_ENABLE_ASSERTS
	#include "Log.h"
	#define HD_CORE_ASSERT(x, ...) { if(x) { HD_CORE_ERROR("Assertion Failed:"); HD_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define HD_ASSERT(x, ...) { if(x) { HD_ERROR("Assertion Failed:"); HD_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define HD_CORE_ASSERT(x, ...)
	#define HD_ASSERT(x, ...)
#endif


#define HD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define GET_COUNT(x) *(&x+1) - x


#include <memory>
namespace Hudi {
	
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> NewScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr Ref<T> NewRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

