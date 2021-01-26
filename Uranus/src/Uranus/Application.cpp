
#include "urpch.h"

#include "Uranus/Application.h"
#include "Uranus/Input.h"

#include <glad/glad.h>

namespace Uranus {

	Application* Application::_Instance;

	static GLenum ShaderDateTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case Uranus::ShaderDataType::Float:		return GL_FLOAT;
			case Uranus::ShaderDataType::Float2:	return GL_FLOAT;
			case Uranus::ShaderDataType::Float3:	return GL_FLOAT;
			case Uranus::ShaderDataType::Float4:	return GL_FLOAT;
			case Uranus::ShaderDataType::Mat3:		return GL_FLOAT;
			case Uranus::ShaderDataType::Mat4:		return GL_FLOAT;
			case Uranus::ShaderDataType::Int:		return GL_INT;
			case Uranus::ShaderDataType::Int2:		return GL_INT;
			case Uranus::ShaderDataType::Int3:		return GL_INT;
			case Uranus::ShaderDataType::Int4:		return GL_INT;
			case Uranus::ShaderDataType::Bool:		return GL_BOOL;
		}

		UR_CORE_ASSERT(false, "Unknown Shader Data Type!")
			return 0;
	}

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


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.0f, 0.3f, 1.0f
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout bufferLayout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(bufferLayout);

		uint8_t index = 0;
		for (auto& element : bufferLayout.GetElements()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, GetShderTypeComponentCount(element.Type),
				ShaderDateTypeToOpenGLBaseType(element.Type) , element.Normlized,
				bufferLayout.GetStride(), (const void*)element.Offset);
			index++;
		}


		uint32_t indeices[3] = { 0, 1, 2 };
		indexBuffer.reset(IndexBuffer::Create(indeices, sizeof(indeices) / sizeof(uint32_t)));

		std::string vertexShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position + 0.5, 1);
				color = v_Color;
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