#pragma once


#ifdef UR_PLATFORM_WINDOWS

extern Uranus::Application* CreateApplication();

int main() {
	auto* app = CreateApplication();
	app->Run();
	delete app;
}
#endif