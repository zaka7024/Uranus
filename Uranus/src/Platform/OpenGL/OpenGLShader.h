#pragma once

#include "Uranus/Renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>

namespace Uranus {

	typedef unsigned int GLenum;

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		~OpenGLShader() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetMat4(const glm::mat4& value, const std::string& name) override;
		virtual void SetFloat(float value, const std::string& name) override;
		virtual void SetFloat4(const glm::vec4& value, const std::string& name) override;
		virtual void SetFloat3(const glm::vec3& value, const std::string& name) override;
		virtual void SetInt(uint32_t value, const std::string& name) override;
		virtual void SetIntArray(int32_t* values, uint32_t count, const std::string& name) override;
		virtual const std::string& GetName() const override;

		inline uint32_t GetProgramId() { return _RendererId; }

		void UploadUniformMat4(const glm::mat4& mat4, const std::string& name);
		void UploadUniformMat3(const glm::mat3& mat3, const std::string& name);
		void UploadUniformFloat4(const glm::vec4& values, const std::string& name);
		void UploadUniformFloat3(const glm::vec3& values, const std::string& name);
		void UploadUniformFloat2(const glm::vec2& values, const std::string& name);
		void UploadUniformFloat(float value, const std::string& name);
		void UploadUniformInt(int value, const std::string& name);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string> shaderSources);

	private:
		uint32_t _RendererId;
		std::string _Name;
	};
}