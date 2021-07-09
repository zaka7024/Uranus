
#include <Uranus.h>
#include "Uranus/Core/EntryPoint.h"
#include <Platform/OpenGL/OpenGLShader.h>

#include "UranusEditorLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Uranus {
	class UranusEditorApp : public Application {

	public:
		UranusEditorApp() : Application("Uranus Editor") {
			PushLayer(new UranusEditorLayer());
		}

		~UranusEditorApp() {

		}
	};
}

Uranus::Application* CreateApplication() {
	return new Uranus::UranusEditorApp();
}
