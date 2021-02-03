#pragma once

#include "urpch.h"

#include "Uranus/Core.h"
#include "Uranus/Log.h"
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
		LayerStack _LayerStack;
		float _LastFrameTime = 0.0f;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication();
}
