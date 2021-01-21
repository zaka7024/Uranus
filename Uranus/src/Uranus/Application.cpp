
#include "urpch.h"

#include "Uranus/Application.h"
#include "Uranus/Input.h"

#include <glad/glad.h>

namespace Uranus {

	Application* Application::_Instance;

	Application::Application()
	{
		UR_CORE_ASSERT(!_Instance, "There exsist already Application instance")
		_Instance = this;

		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(UR_BIND_EVENT_FUN(Application::OnEvent));
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(UR_BIND_EVENT_FUN(Application::OnWindowClose));

		// UR_CORE_TRACE("{0}", e);

		for (auto item = _layerStack.end(); item != _layerStack.begin(); ) {
			(*--item)->OnEvent(e);
			if (e.isHandled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (_IsRunning) {

			_Window->OnUpdate();

			for (Layer* layer : _layerStack)
				layer->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		_IsRunning = false;
		return true;
	}
}