#include "urpch.h"

#include "Uranus/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Uranus/Application.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/MouseEvent.h"

#include "GLFW/glfw3.h"

namespace Uranus {

    ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
        ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
		
        // Initilize ImGui with OpenGL
        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {

	}

	void ImGuiLayer::OnUpdate() {

        //UR_CORE_INFO("ImGuiLayer::OnUpdate")

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = time > 0.0f ? (time - _time) : (.0f / 60.0f);
        _time = time;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void ImGuiLayer::OnEvent(Event& event) {

        if (event.GetEventType() == Uranus::EventType::KeyPressed) {
            ImGuiIO& io = ImGui::GetIO();

            KeyPressedEvent& e = ((Uranus::KeyPressedEvent&)event);
            io.KeysDown[e.GetKeyCode()] = true;
        }

        if (event.GetEventType() == Uranus::EventType::MouseButtonPressed) {
            ImGuiIO& io = ImGui::GetIO();
            MouseButtonPressedEvent& e = ((Uranus::MouseButtonPressedEvent&)event);
            io.MouseDown[e.GetMouseButton()] = true;
        }

        if (event.GetEventType() == Uranus::EventType::MouseMoved) {
            ImGuiIO& io = ImGui::GetIO();
            MouseMovedEvent& e = ((Uranus::MouseMovedEvent&)event);
            io.MousePos = ImVec2(e.GetX(), e.GetY());
        }
	}
}
