#pragma once
#include <memory>

#ifdef UR_PLATFORM_WINDOWS
	#ifdef UR_DYNAMIC_LINK
		#ifdef UR_BUILD_DLL
			#define URANUS_API _declspec(dllexport)
		#else
			#define URANUS_API _declspec(dllimport)
		#endif // UR_BUILD_DLL
	#else
		#define URANUS_API
	#endif
#else
	#error Uranus only supports Windows
#endif

#ifdef UR_DEBUG
	#define UR_ENABLE_ASSERT
#endif // UR_DEBUG



#ifdef UR_ENABLE_ASSERT
	#define UR_ASSERT(x, y) { if(!(x)) { UR_ERROR("Assertion Failed: {0}", y); __debugbreak(); } }
	#define UR_CORE_ASSERT(x, y) { if(!(x)) { UR_CORE_ERROR("Assertion Failed: {0}", y); __debugbreak(); } }
#else
	#define UR_ASSERT (x, ...)
	#define UR_CORE_ASSERT(x, ...) 

#endif // UR_ENABLE_ASSERT


#define BIT(x) 1 << x

#define UR_BIND_EVENT_FUN(x) std::bind(&x, this, std::placeholders::_1)

namespace Uranus {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}
