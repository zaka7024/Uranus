
#include <Uranus.h>

class Sandbox : public Uranus::Application {

};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}