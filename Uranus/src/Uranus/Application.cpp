
#include "urpch.h"

#include "Uranus/Application.h"
#include "Uranus/Input.h"
#include "Renderer/Renderer.h"
#include "Uranus/KeyCodes.h"

#include "Uranus/Core/Timestep.h"

#include<GLFW/glfw3.h>

namespace Uranus {

	Application* Application::_Instance;

	Application::Application()
	{
		UR_CORE_ASSERT(!_Instance, "There exsist already Application instance")
		_Instance = this;

		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(UR_BIND_EVENT_FUN(Application::OnEvent));

		Renderer::Init();

		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(UR_BIND_EVENT_FUN(Application::OnWindowClose));
		eventDispatcher.Dispatch<WindowResizeEvent>(UR_BIND_EVENT_FUN(Application::OnWindowResize));

		// UR_CORE_TRACE("{0}", e);

		for (auto item = _LayerStack.end(); item != _LayerStack.begin(); ) {
			(*--item)->OnEvent(e);
			if (e.isHandled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		static float z, y, x, r;

		while (_IsRunning) {

			float time = (float)glfwGetTime();
			Timestep ts = time - _LastFrameTime;
			_LastFrameTime = time;

			if (!_Minimzed) {
				for (Layer* layer : _LayerStack)
					layer->OnUpdate(ts);
			}

			_ImGuiLayer->Begin();
			for (Layer* layer : _LayerStack)
				layer->OnImGuiRender();
			_ImGuiLayer->End();

			_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			_Minimzed = true;
			return false;
		}

		_Minimzed = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}