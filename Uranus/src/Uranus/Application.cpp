
#include "urpch.h"

#include "Uranus/Application.h"
#include "Uranus/Input.h"

#include "Renderer/Renderer.h"

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
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->Bind();

		float vertices[4 * 7] = {
			-0.8f, -0.8f,  0.0f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.8f, -0.8f,  0.0f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.8f,  0.8f,  0.0f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.8f,  0.8f,  0.0f,  0.2f, 0.3f, 0.9f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout bufferLayout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(bufferLayout);
		_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indeices[6] = { 0, 1, 2, 1, 3, 2};
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indeices, sizeof(indeices) / sizeof(uint32_t)));

		_VertexArray->SetIndexBuffer(indexBuffer);

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

		_Shader = std::make_shared<Shader>(vertexShader, fragmentShader);

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

			Renderer::BeginScene();

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1});
			RenderCommand::Clear();

			_Shader->Bind();
			Renderer::Submit(_VertexArray);

			Renderer::EndScene();

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