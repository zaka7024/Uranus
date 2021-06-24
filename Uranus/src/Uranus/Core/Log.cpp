
#include "urpch.h"

#include "Log.h"

namespace Uranus {

	std::shared_ptr<spdlog::logger> Log::s_clientLogger;
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreLogger = spdlog::stdout_color_mt("URANUS");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	}

}
