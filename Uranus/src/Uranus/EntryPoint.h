#pragma once

#ifdef UR_PLATFORM_WINDOWS

extern Uranus::Application* CreateApplication();

int main() {

	Uranus::Log::Init();
	UR_CORE_INFO("Hello, From Uranus");
	UR_ERROR("Can not divide by {0}", 0)

	auto* app = CreateApplication();
	app->Run();
	delete app;
}
#endif