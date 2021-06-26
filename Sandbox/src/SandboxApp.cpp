
#include <Uranus.h>
#include "Uranus/Core/EntryPoint.h"
#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Uranus::Layer {

public:
	ExampleLayer()
		:Layer("ExampleLayer"), _CameraPosition(0.0f), _CameraController(1280.0f/ 720.0f, true),
		_Transfrom(0.0f) {

		_VertexArray = Uranus::VertexArray::Create();
		_VertexArray->Bind();

		float vertices[4 * 5] = {
			-0.1f, -0.1f,  0.1f, 0.0f, 0.0f,
			 0.1f, -0.1f,  0.1f, 1.0f, 0.0f,
			-0.1f,  0.1f,  0.1f, 0.0f, 1.0f,
			 0.1f,  0.1f,  0.1f, 1.0f, 1.0f
		};

		Uranus::Ref<Uranus::VertexBuffer> vertexBuffer;
		vertexBuffer = Uranus::VertexBuffer::Create(vertices, sizeof(vertices));

		Uranus::BufferLayout bufferLayout = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(bufferLayout);
		_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indeices[6] = { 0, 1, 2, 1, 3, 2 };
		Uranus::Ref<Uranus::IndexBuffer> indexBuffer;
		indexBuffer= Uranus::IndexBuffer::Create(indeices, sizeof(indeices) / sizeof(uint32_t));

		_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transfrom;

			out vec3 v_Position;
			out vec2 v_TexCoord;

			void main() {
				v_Position = a_Position;
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transfrom * vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;
			uniform sampler2D u_Texture;
		
			in vec2 v_TexCoord;
			in vec3 v_Position;

			void main() {
				color = texture(u_Texture, v_TexCoord) * u_Color;
			}

		)";

		_Shader = Uranus::Shader::Create("", vertexShader, fragmentShader);


		// Blue
		_VertexArrayBlue = Uranus::VertexArray::Create();
		_VertexArrayBlue->Bind();

		float verticesBlue[4 * 7] = {
			-0.2f, -0.2f,  0.6f,  0.007f, 0.27f, 0.9f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.007f, 0.27f, 0.9f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.007f, 0.27f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.007f, 0.27f, 0.9f, 1.0f
		};
		 
		Uranus::Ref<Uranus::VertexBuffer> vertexBufferBlue;
		vertexBufferBlue = Uranus::VertexBuffer::Create(verticesBlue, sizeof(verticesBlue));

		Uranus::BufferLayout bufferLayoutBlue = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float4, "a_Color"}
		};

		vertexBufferBlue->SetLayout(bufferLayoutBlue);
		_VertexArrayBlue->AddVertexBuffer(vertexBufferBlue);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		Uranus::Ref<Uranus::IndexBuffer> indexBufferBlue;
		indexBufferBlue = (Uranus::IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

		_VertexArrayBlue->SetIndexBuffer(indexBufferBlue);

		auto shaderBlue = _ShaderLibrary.Load("assets/shaders/shader.glsl");

		_Texture = Uranus::Texture2D::Create("assets/textures/moon.png");
	}

	void OnUpdate(Uranus::Timestep ts) {

		_CameraController.OnUpdate(ts);

		Uranus::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Uranus::RenderCommand::Clear();

		Uranus::Renderer::BeginScene(_CameraController.GetCamera());


		if (Uranus::Input::IsKeyPressed(UR_KEY_UP))
			_Transfrom.y += _SquareMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_DOWN))
			_Transfrom.y -= _SquareMoveSpeed * ts;

		if (Uranus::Input::IsKeyPressed(UR_KEY_RIGHT))
			_Transfrom.x += _SquareMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_LEFT))
			_Transfrom.x -= _SquareMoveSpeed * ts;

		if (Uranus::Input::IsKeyPressed(UR_KEY_J)) {
			_Rotation += _SquareRotationSpeed * ts;
		}

		if (Uranus::Input::IsKeyPressed(UR_KEY_L)) {
			_Rotation -= _SquareRotationSpeed * ts;
		}

		glm::mat4 transformer(1.0f);
		transformer = glm::translate(transformer, _Transfrom);
		transformer = glm::rotate(transformer, _Rotation, { 0, 0, 1 });

		_Shader->Bind();
		_Texture->Bind();
		
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(_Shader)->UploadUniformFloat4(_Color, "u_Color");
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(_Shader)->UploadUniformInt(0, "u_Texture");

		auto shaderBlue = _ShaderLibrary.Get("shader");

		Uranus::Renderer::Submit(shaderBlue, _VertexArrayBlue);
		Uranus::Renderer::Submit(_Shader, _VertexArray, transformer);

		Uranus::Renderer::EndScene();
	}

	void OnEvent(Uranus::Event& event) {
		_CameraController.OnEvent(event);
	}

	void OnImGuiRender() {
		ImGui::Begin("Window");
		ImGui::Text("Hello, World");
		ImGui::End();

		ImGui::Begin("Color Picker");
		ImGui::ColorEdit4("Color", glm::value_ptr(_Color));
		ImGui::End();
	}

private:
	Uranus::Ref<Uranus::VertexArray> _VertexArray;

	Uranus::Ref<Uranus::VertexArray> _VertexArrayBlue;
	Uranus::Ref<Uranus::Shader> _Shader;
	Uranus::Ref<Uranus::Texture2D> _Texture;
	Uranus::ShaderLibrary _ShaderLibrary;

	Uranus::OrthographicCameraController _CameraController;

	glm::vec3 _CameraPosition;
	float _CameraMoveSpeed = 3.0f;
	float _SquareMoveSpeed = 2.5f;
	float _SquareRotationSpeed = 5.0f;

	glm::vec3 _Transfrom;
	glm::vec4 _Color = {1.0f, 1.0f, 1.0f, 1.0f};
	float _Rotation = 0.0f;
};

class Sandbox : public Uranus::Application {

public:
	Sandbox() {
		PushLayer(new Sandbox2D());
		//PushLayer(new ExampleLayer());
		//PushOverlay(new Uranus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}