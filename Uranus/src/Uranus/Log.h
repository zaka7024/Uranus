#pragma once

#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Uranus {

	class URANUS_API Log
	{

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_coreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_clientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;

	};

}

// Core logs macros

#define UR_CORE_TRACE(...)	Uranus::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define UR_CORE_INFO(...)	Uranus::Log::GetCoreLogger()->info(__VA_ARGS__);
#define UR_CORE_ERROR(...)	Uranus::Log::GetCoreLogger()->error(__VA_ARGS__);
#define UR_CORE_WARN(...)	Uranus::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define UR_CORE_FATAL(...)	Uranus::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// Client logs macros

#define UR_TRACE(...)	Uranus::Log::GetClientLogger()->trace(__VA_ARGS__);
#define UR_INFO(...)	Uranus::Log::GetClientLogger()->info(__VA_ARGS__);
#define UR_ERROR(...)	Uranus::Log::GetClientLogger()->error(__VA_ARGS__);
#define UR_WARN(...)	Uranus::Log::GetClientLogger()->warn(__VA_ARGS__);
#define UR_FATAL(...)	Uranus::Log::GetClientLogger()->fatal(__VA_ARGS__);