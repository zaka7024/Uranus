#pragma once

#ifdef UR_PLATFORM_WINDOWS

extern Uranus::Application* CreateApplication();

int main() {

	Uranus::Log::Init();

	UR_PROFILE_BEGIN_SESSION("Startup", "UranusProfile-Startup.json");
	auto* app = CreateApplication();
	UR_PROFILE_END_SESSION();

	UR_PROFILE_BEGIN_SESSION("Runtime", "UranusProfile-Runtime.json");
	app->Run();
	UR_PROFILE_END_SESSION();

	UR_PROFILE_BEGIN_SESSION("Shutdown", "UranusProfile-Shutdown.json");
	delete app;
	UR_PROFILE_END_SESSION();
}
#endif