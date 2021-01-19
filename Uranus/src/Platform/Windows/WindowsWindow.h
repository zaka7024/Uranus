#pragma once

#include "Uranus/Window.h"
#include "GLFW/glfw3.h"

namespace Uranus {

	class WindowsWindow : public Window {
	public:

		WindowsWindow(const WindowProps& windowProps);

		void OnUpdate() override;


		inline unsigned int GetWidth() const override{ return _WindowData.Width; };
		inline unsigned int GetHeight() const override { return _WindowData.Height; }

		inline void SetEventCallback(const EventCallbackFun& callback) override { _WindowData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

	private:

		void Init(const WindowProps& windowProps);
		void Shutdown();

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFun EventCallback;
		};

		WindowData _WindowData;

		GLFWwindow* _Window;
	};

}