#include "urpch.h"

#include "Uranus/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Uranus/Application.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/MouseEvent.h"

// Temp
#include "GLFW/glfw3.h"
#include <glad/glad.h>

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

        static bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void ImGuiLayer::OnEvent(Event& event) {

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonRelesedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnMouseButtonRelesedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<WindowResizeEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<KeyPressedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(UR_BIND_EVENT_FUN(ImGuiLayer::OnKeyTypedEvent));
	}

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonRelesedEvent(MouseButtonRelesedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetOffsetX();
        io.MouseWheel += e.GetOffsetY();

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_RIGHT_CONTROL] || io.KeysDown[GLFW_KEY_LEFT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_RIGHT_SHIFT] || io.KeysDown[GLFW_KEY_LEFT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_RIGHT_ALT] || io.KeysDown[GLFW_KEY_LEFT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_RIGHT_SUPER] || io.KeysDown[GLFW_KEY_LEFT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(e.GetKeyCode());

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
       
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }
}
