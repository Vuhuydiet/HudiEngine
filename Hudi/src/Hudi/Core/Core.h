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

