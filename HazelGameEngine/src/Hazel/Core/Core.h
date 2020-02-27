#pragma once
#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HZ_API __declspec(dllexport)
	#else	
		#define HZ_API __declspec(dllimport)
	#endif
#else
	#define  HZ_API
#endif
#else
	#error Hazel only for windows now
#endif

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)

//This is for event bind mapping
#define HZ_BIND_EVENT_FN(function) std::bind(&function, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}