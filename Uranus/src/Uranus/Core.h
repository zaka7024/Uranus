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

#define BIT(x) 1 << x
