
workspace "Uranus"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Uranus/vendor/GLFW/include"
IncludeDir["Glad"] = "Uranus/vendor/glad/include"
IncludeDir["ImGui"] = "Uranus/vendor/imgui"
IncludeDir["glm"] = "Uranus/vendor/glm"
IncludeDir["stb_image"] = "Uranus/vendor/stb_image"
IncludeDir["entt"] = "Uranus/vendor/entt/include"

include "Uranus/vendor/GLFW"
include "Uranus/vendor/Glad"
include "Uranus/vendor/imgui"

project "Uranus"
	location "Uranus"
	staticruntime "on"
	kind "StaticLib"
	cppdialect "C++17"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "urpch.h"
	pchsource "Uranus/src/urpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/**.hpp",
		"%{prj.name}/vendor/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"UR_PLATFORM_WINDOWS",
			"UR_DEBUG",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "UR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "UR_RELAESE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "UR_DIST"
		runtime "Release"
		optimize "on"
		
project "UranusEditor"
	location "UranusEditor"
	kind "ConsoleApp"
	staticruntime "on"
	cppdialect "C++17"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Uranus/src",
		"Uranus/vendor/spdlog/include",
		"Uranus/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links {
		"Uranus"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"UR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "UR_RELAESE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "UR_DIST"
		runtime "Release"
		optimize "On"
		

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"
	cppdialect "C++17"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Uranus/src",
		"Uranus/vendor/spdlog/include",
		"Uranus/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links {
		"Uranus"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"UR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "UR_RELAESE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "UR_DIST"
		runtime "Release"
		optimize "On"
		