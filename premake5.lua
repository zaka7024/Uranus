
workspace "Uranus"

	architecture "x64"	

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Uranus/vendor/GLFW/include"
IncludeDir["Glad"] = "Uranus/vendor/glad/include"

include "Uranus/vendor/GLFW"
include "Uranus/vendor/Glad"

project "Uranus"
	location "Uranus"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "urpch.h"
	pchsource "Uranus/src/urpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"Glad"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"UR_PLATFORM_WINDOWS",
			"UR_BUILD_DLL",
			"UR_ENABLE_ASSERT",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "UR_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "UR_RELAESE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "UR_DIST"
		buildoptions "/MD"
		optimize "On"
		

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Uranus/src",
		"Uranus/vendor/spdlog/include"
	}

	links {
		"Uranus"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"UR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UR_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "UR_RELAESE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "UR_DIST"
		buildoptions "/MD"
		optimize "On"
		