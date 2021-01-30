
#include <Uranus.h>

#include "imgui/imgui.h"

class ExampleLayer : public Uranus::Layer {

public:
	ExampleLayer()
		:Layer("ExampleLayer"), _CameraPosition(0.0f), _Camera(-1.6f, 1.6f, -0.9f, 0.9f) {
		// OpenGL Code
		_VertexArray.reset(Uranus::VertexArray::Create());
		_VertexArray->Bind();

		float vertices[4 * 7] = {
			-0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f
		};

		std::shared_ptr<Uranus::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Uranus::VertexBuffer::Create(vertices, sizeof(vertices)));

		Uranus::BufferLayout bufferLayout = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(bufferLayout);
		_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indeices[6] = { 0, 1, 2, 1, 3, 2 };
		std::shared_ptr<Uranus::IndexBuffer> indexBuffer;
		indexBuffer.reset(Uranus::IndexBuffer::Create(indeices, sizeof(indeices) / sizeof(uint32_t)));

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

		_Shader = std::make_shared<Uranus::Shader>(vertexShader, fragmentShader);

		// Blue
		_VertexArrayBlue.reset(Uranus::VertexArray::Create());
		_VertexArrayBlue->Bind();

		float verticesBlue[4 * 7] = {
			-0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f
		};

		std::shared_ptr<Uranus::VertexBuffer> vertexBufferBlue;
		vertexBufferBlue.reset(Uranus::VertexBuffer::Create(verticesBlue, sizeof(verticesBlue)));

		Uranus::BufferLayout bufferLayoutBlue = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float4, "a_Color"}
		};

		vertexBufferBlue->SetLayout(bufferLayoutBlue);
		_VertexArrayBlue->AddVertexBuffer(vertexBufferBlue);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		std::shared_ptr<Uranus::IndexBuffer> indexBufferBlue;
		indexBufferBlue.reset(Uranus::IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

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

		_ShaderBlue = std::make_shared<Uranus::Shader>(vertexShaderBlue, fragmentShaderBlue);
	}

	void OnUpdate(Uranus::Timestep ts) {

		UR_TRACE("Deleta Time: {0}s, {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());

		Uranus::Renderer::BeginScene(_Camera);

		Uranus::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Uranus::RenderCommand::Clear();

		if (Uranus::Input::IsKeyPressed(UR_KEY_W))
			_CameraPosition.y += _CameraMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_S))
			_CameraPosition.y -= _CameraMoveSpeed * ts;

		if (Uranus::Input::IsKeyPressed(UR_KEY_D))
			_CameraPosition.x += _CameraMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_A))
			_CameraPosition.x -= _CameraMoveSpeed * ts;

		_Camera.SetPoisition(_CameraPosition);
		_Camera.SetRotation(0.0f);

		Uranus::Renderer::Submit(_ShaderBlue, _VertexArrayBlue);
		Uranus::Renderer::Submit(_Shader, _VertexArray);

		Uranus::Renderer::EndScene();
	}

	void OnEvent(Uranus::Event& event) {
		
	}

	void OnImGuiRender() {
		ImGui::Begin("Window");
		ImGui::Text("Hello, World");
		ImGui::End();
	}

private:
	std::shared_ptr<Uranus::VertexArray> _VertexArray;
	std::shared_ptr<Uranus::Shader> _Shader;

	std::shared_ptr<Uranus::VertexArray> _VertexArrayBlue;
	std::shared_ptr<Uranus::Shader> _ShaderBlue;

	Uranus::OrthographicCamera _Camera;

	glm::vec3 _CameraPosition;
	float _CameraMoveSpeed = 3.0f;
};

class Sandbox : public Uranus::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//PushOverlay(new Uranus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}