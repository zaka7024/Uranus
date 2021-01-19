#pragma once

#ifdef UR_PLATFORM_WINDOWS
	#ifdef UR_BUILD_DLL
		#define URANUS_API _declspec(dllexport)
	#else
		#define URANUS_API _declspec(dllimport)
	#endif // UR_BUILD_DLL
#else
	#error Uranus only supports Windows
#endif

#ifdef UR_ENABLE_ASSERT
	#define UR_ASSERT(x, y) { if(!(x)) { UR_ERROR("Assertion Failed: {0}", y); __debugbreak(); } }
	#define UR_CORE_ASSERT(x, y) { if(!(x)) { UR_CORE_ERROR("Assertion Failed: {0}", y); __debugbreak(); } }
#else
	#define UR_ASSERT (x, ...)
	#define UR_CORE_ASSERT(x, ...) 

#endif // UR_ENABLE_ASSERT


#define BIT(x) 1 << x
