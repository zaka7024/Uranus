#include "urpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Uranus {
	
	Shader* Shader::Create(std::string& vertexSrc, std::string& fragmentSrc) {
		return new OpenGLShader(vertexSrc, fragmentSrc);
	}

}
