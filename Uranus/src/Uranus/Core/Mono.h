#pragma once

#include <windows.h>
#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include <cstdlib>
#include <string>
#include <iostream>

namespace Uranus {
	namespace Mono {
		static bool Init() {
			return false;
        }
	}
}