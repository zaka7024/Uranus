
#include "urpch.h"
#include "WindowsWindow.h"

#include "Uranus/Events/ApplicationEvent.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/MouseEvent.h"

#include "glad/glad.h"

namespace Uranus {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& windowProps) {
		return new WindowsWindow(windowProps);
	}

	WindowsWindow::WindowsWindow(const WindowProps& windowProps) {
		Init(windowProps);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}

	void WindowsWindow::Init(const WindowProps& windowProps) {
		_WindowData.Title = windowProps.Name;
		_WindowData.Height = windowProps.Height;
		_WindowData.Width = windowProps.Width;

		UR_CORE_INFO("Creating window {0} ({1}, {2})", _WindowData.Title, _WindowData.Height, _WindowData.Width);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			UR_CORE_ASSERT(true, "Could not initilize GLFW!");
			s_GLFWInitialized = true;
		}

		_Window = glfwCreateWindow(_WindowData.Width, _WindowData.Height, _WindowData.Title.c_str(), nullptr, nullptr);
		
		// Craete OpenGL Context
		glfwMakeContextCurrent(_Window);
		
		// Initilize Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		UR_CORE_ASSERT(status, "Could not initilize Glad!")

		glfwSetWindowUserPointer(_Window, &_WindowData);
		SetVSync(true);

		// Set GLFW events callbacks
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.Width = width;
			windowData.Height = height;

			WindowResizeEvent event(width, height);
			windowData.EventCallback(event);
		});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			windowData.EventCallback(event);
		});

		glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					windowData.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonRelesedEvent event(button);
					windowData.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			windowData.EventCallback(event);

		});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			windowData.EventCallback(event);

		});
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		_WindowData.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() {
		return false;
	}
}