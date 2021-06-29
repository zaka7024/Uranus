#include "urpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Uranus/Core/Core.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Uranus {
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetApi())
		{
		case RendererAPI::API::None: UR_CORE_ASSERT(false, "RendererAPI::None is currently not suported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
	}

	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetApi())
		{
		case RendererAPI::API::None: UR_CORE_ASSERT(false, "RendererAPI::None is currently not suported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		UR_CORE_ASSERT(!Exists(name), "Shader already exists!");
		_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		UR_CORE_ASSERT(Exists(name), "Shader not found!");
		return _Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return _Shaders.find(name) != _Shaders.end();
	}
}
