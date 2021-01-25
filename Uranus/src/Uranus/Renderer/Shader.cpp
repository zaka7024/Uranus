#include "urpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Uranus {
	Shader::Shader(std::string& vertexSrc, std::string& fragmentSrc)
	{

		// Create an empty vertex shader handle

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			UR_CORE_ERROR("{0}", infoLog.data());
			UR_CORE_ASSERT(false, "Vertex Shader Compilation Error.")
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			UR_CORE_ERROR("{0}", infoLog.data());
			UR_CORE_ASSERT(false, "Fragment Shader Compilation Error.")
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_RendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(_RendererId, vertexShader);
		glAttachShader(_RendererId, fragmentShader);

		// Link our program
		glLinkProgram(_RendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			UR_CORE_ERROR("{0}", infoLog.data());
			UR_CORE_ASSERT(false, "Shader Linking error.")
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(_RendererId, vertexShader);
		glDetachShader(_RendererId, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(_RendererId);
	}

	void Shader::Bind()
	{
		glUseProgram(_RendererId);
	}
	
	void Shader::Unbind()
	{
		glUseProgram(0);
	}
}
