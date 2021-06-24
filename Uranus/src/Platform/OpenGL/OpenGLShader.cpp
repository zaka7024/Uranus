#include "urpch.h"
#include "OpenGLShader.h"

#include<array>
#include<fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Uranus {


	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		UR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:_Name(name) 
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string& fileSource = ReadFile(filepath);
		std::unordered_map<GLenum, std::string>& shadersSources = PreProcess(fileSource);
		Compile(shadersSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		int count = lastDot == std::string::npos ? filepath.size() - lastSlash: lastDot - lastSlash;
		_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_RendererId);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(_RendererId);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return _Name;
	}

	void OpenGLShader::UploadUniformMat4(const glm::mat4& mat4, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void OpenGLShader::UploadUniformMat3(const glm::mat3& mat3, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void OpenGLShader::UploadUniformFloat4(const glm::vec4& values, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformFloat3(const glm::vec3& values, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat2(const glm::vec2& values, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat(float value, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformInt(int value, const std::string& name)
	{
		GLuint location = glGetUniformLocation(_RendererId, name.c_str());
		glUniform1i(location, value);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string source;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			source.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&source[0], source.size());
			in.close();
		}
		else {
			UR_CORE_ERROR("Could not open file '{0}'", filepath)
		}

		return source;
	}
	
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			UR_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			UR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram();
		_RendererId = program;
		std::array<GLenum, 2> shadersIds;

		UR_CORE_ASSERT(shaderSources.size() <= shadersIds.size(), "We just support 2 shaders");

		int shaderIdIndex = 0;
		for (auto& kv : shaderSources) {

			GLenum shaderType = kv.first;
			const std::string& shaderSource = kv.second;

			GLuint shader = glCreateShader(shaderType);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				UR_CORE_ERROR("{0}", infoLog.data());
				UR_CORE_ASSERT(false, "Vertex Shader Compilation Error.")
					return;
			}

			shadersIds[shaderIdIndex++] = shader;

			// Attach our shaders to our program
			glAttachShader(_RendererId, shader);
		}

		// Link our program
		glLinkProgram(_RendererId);

		GLint isLinked = 0;
		glGetProgramiv(_RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_RendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_RendererId);
			// Don't leak shaders either.
			for (auto id : shadersIds) {
				glDeleteShader(id);
			}

			UR_CORE_ERROR("{0}", infoLog.data());
			UR_CORE_ASSERT(false, "Shader Linking error.")
				return;
		}

		for (auto id : shadersIds) {
			glDetachShader(_RendererId, id);
		}
	}
}