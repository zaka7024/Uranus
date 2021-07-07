#pragma once

#include "urpch.h"

#include "Uranus/Core/Core.h"
#include "Uranus/Core/Log.h"
#include "Uranus/Events/Event.h"
#include "Uranus/Events/ApplicationEvent.h"
#include "Window.h"

#include "LayerStack.h"

#include "Uranus/ImGui/ImGuiLayer.h"

namespace Uranus {

	class URANUS_API Application
	{
	public:
		Application();
		~Application();

		void Run();
		void Close();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *_Instance; }

		inline Window& GetWindow() { return (*Get()._Window); }

	private:
		static Application* _Instance;
		Scope<Window> _Window;
		ImGuiLayer* _ImGuiLayer;
		bool _IsRunning = true;
		bool _Minimzed = false;
		LayerStack _LayerStack;
		float _LastFrameTime = 0.0f;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	Application* CreateApplication();
}
