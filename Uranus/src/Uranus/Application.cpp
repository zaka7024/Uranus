
#include "urpch.h"

#include "Uranus/Application.h"
#include "Uranus/Input.h"

#include "Renderer/Renderer.h"

#include "Uranus/KeyCodes.h"

namespace Uranus {

	Application* Application::_Instance;

	Application::Application()
		:_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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
			-0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f
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

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0) + vec4(-0.6, 0, 0,0);
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

		// Blue
		_VertexArrayBlue.reset(VertexArray::Create());
		_VertexArrayBlue->Bind();

		float verticesBlue[4 * 7] = {
			-0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBufferBlue;
		vertexBufferBlue.reset(VertexBuffer::Create(verticesBlue, sizeof(verticesBlue)));

		BufferLayout bufferLayoutBlue = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};

		vertexBufferBlue->SetLayout(bufferLayoutBlue);
		_VertexArrayBlue->AddVertexBuffer(vertexBufferBlue);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		std::shared_ptr<IndexBuffer> indexBufferBlue;
		indexBufferBlue.reset(IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

		_VertexArrayBlue->SetIndexBuffer(indexBufferBlue);

		std::string vertexShaderBlue = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentShaderBlue = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position + 0.5, 1);
				color = v_Color;
			}

		)";

		_ShaderBlue = std::make_shared<Shader>(vertexShaderBlue, fragmentShaderBlue);

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
		static float z, y, x, r;

		while (_IsRunning) {

			Renderer::BeginScene(_Camera);

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1});
			RenderCommand::Clear();

			if (Input::IsKeyPressed(UR_KEY_UP)) {
				z += 0.05f;
			}
			if (Input::IsKeyPressed(UR_KEY_DOWN)) {
				z -= 0.05f;
			}

			if (Input::IsKeyPressed(UR_KEY_LEFT)) {
				x -= 0.05f;
			}
			if (Input::IsKeyPressed(UR_KEY_RIGHT)) {
				x += 0.05f;
			}

			if (Input::IsKeyPressed(UR_KEY_W)) {
				y += 0.05f;
			}
			if (Input::IsKeyPressed(UR_KEY_S)) {
				y -= 0.05f;
			}

			if (Input::IsKeyPressed(UR_KEY_D)) {
				r += 0.5f;
			}
			if (Input::IsKeyPressed(UR_KEY_A)) {
				r -= 0.5f;
			}

			_Camera.SetPoisition({ x, y, z });
			_Camera.SetRotation(r);

			Renderer::Submit(_ShaderBlue, _VertexArrayBlue);
			Renderer::Submit(_Shader, _VertexArray);

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