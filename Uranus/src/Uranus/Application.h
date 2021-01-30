#pragma once

#include "urpch.h"

#include "Uranus/Core.h"
#include "Uranus/Log.h"
#include "Uranus/Events/Event.h"
#include "Uranus/Events/ApplicationEvent.h"
#include "Window.h"

#include "LayerStack.h"

#include "Uranus/ImGui/ImGuiLayer.h"
#include "Uranus/Renderer/Shader.h"
#include "Uranus/Renderer/Buffer.h"
#include "Uranus/Renderer/VertexArray.h"

#include "Uranus/Renderer/OrthographicCamera.h"

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
		std::unique_ptr<Window> _Window;
		ImGuiLayer* _ImGuiLayer;
		bool _IsRunning = true;
		LayerStack _LayerStack;

		std::shared_ptr<VertexArray> _VertexArray;
		std::shared_ptr<Shader> _Shader;

		std::shared_ptr<VertexArray> _VertexArrayBlue;
		std::shared_ptr<Shader> _ShaderBlue;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		OrthographicCamera _Camera;
	};

	Application* CreateApplication();
}
