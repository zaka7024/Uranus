
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

		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);

		// OpenGL Code
		glGenVertexArrays(1, &_VertexArray);
		glBindVertexArray(_VertexArray);

		glGenBuffers(1, &_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);

		float vertieces[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertieces), vertieces, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);

		unsigned int indeices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeices), indeices, GL_STATIC_DRAW);

		std::string vertexShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main() {
				color = vec4(v_Position + 0.5, 1);
			}

		)";

		shader = std::make_unique<Shader>(vertexShader, fragmentShader);

	}

	void Application::OnEvent(Event& e) {

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(UR_BIND_EVENT_FUN(Application::OnWindowClose));

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
		while (_IsRunning) {

			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->Bind();

			glBindVertexArray(_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : _LayerStack)
				layer->OnUpdate();

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
}