
#include <Uranus.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Uranus::Layer {

public:
	ExampleLayer()
		:Layer("ExampleLayer"), _CameraPosition(0.0f), _Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		_Transfrom(0.0f) {
		// OpenGL Code
		_VertexArray.reset(Uranus::VertexArray::Create());
		_VertexArray->Bind();

		float vertices[4 * 5] = {
			-0.6f, -0.6f,  0.6f, 0.0f, 0.0f,
			 0.6f, -0.6f,  0.6f, 1.0f, 0.0f,
			-0.6f,  0.6f,  0.6f, 0.0f, 1.0f,
			 0.6f,  0.6f,  0.6f, 1.0f, 1.0f
		};

		Uranus::Ref<Uranus::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Uranus::VertexBuffer::Create(vertices, sizeof(vertices)));

		Uranus::BufferLayout bufferLayout = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(bufferLayout);
		_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indeices[6] = { 0, 1, 2, 1, 3, 2 };
		Uranus::Ref<Uranus::IndexBuffer> indexBuffer;
		indexBuffer.reset(Uranus::IndexBuffer::Create(indeices, sizeof(indeices) / sizeof(uint32_t)));

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

		_Shader.reset(Uranus::Shader::Create(vertexShader, fragmentShader));

		// Blue
		_VertexArrayBlue.reset(Uranus::VertexArray::Create());
		_VertexArrayBlue->Bind();

		float verticesBlue[4 * 7] = {
			-0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			 0.2f, -0.2f,  0.6f,  0.9f, 0.2f, 0.3f, 1.0f,
			-0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f,
			 0.2f,  0.2f,  0.6f,  0.2f, 0.3f, 0.9f, 1.0f
		};

		Uranus::Ref<Uranus::VertexBuffer> vertexBufferBlue;
		vertexBufferBlue.reset(Uranus::VertexBuffer::Create(verticesBlue, sizeof(verticesBlue)));

		Uranus::BufferLayout bufferLayoutBlue = {
			{ Uranus::ShaderDataType::Float3, "a_Position"},
			{ Uranus::ShaderDataType::Float4, "a_Color"}
		};

		vertexBufferBlue->SetLayout(bufferLayoutBlue);
		_VertexArrayBlue->AddVertexBuffer(vertexBufferBlue);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		Uranus::Ref<Uranus::IndexBuffer> indexBufferBlue;
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
				v_Color = a_Color * vec4(a_Position, 1.0);
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

		_ShaderBlue.reset(Uranus::Shader::Create(vertexShaderBlue, fragmentShaderBlue));

		_Texture = Uranus::Texture2D::Create("assets/textures/clash.jpg");
	}

	void OnUpdate(Uranus::Timestep ts) {

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


		_Camera.SetPoisition(_CameraPosition);
		_Camera.SetRotation(0.0f);

		//_Rotation += _SquareRotationSpeed * ts;

		glm::mat4 transformer(1.0f);
		transformer = glm::translate(transformer, _Transfrom);
		transformer = glm::rotate(transformer, _Rotation, { 0, 0, 1 });

		_Shader->Bind();
		_Texture->Bind();
		
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(_Shader)->UploadUniformFloat4(_Color, "u_Color");
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(_Shader)->UploadUniformInt(0, "u_Texture");

		Uranus::Renderer::Submit(_Shader, _VertexArray, transformer);
		//Uranus::Renderer::Submit(_ShaderBlue, _VertexArrayBlue);

		Uranus::Renderer::EndScene();
	}

	void OnEvent(Uranus::Event& event) {
		
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
	Uranus::Ref<Uranus::Shader> _Shader;

	Uranus::Ref<Uranus::VertexArray> _VertexArrayBlue;
	Uranus::Ref<Uranus::Shader> _ShaderBlue;
	Uranus::Ref<Uranus::Texture2D> _Texture;

	Uranus::OrthographicCamera _Camera;

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
		PushLayer(new ExampleLayer());
		//PushOverlay(new Uranus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}