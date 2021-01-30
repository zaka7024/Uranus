#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Uranus {

	class Shader {
	public:
		Shader(std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();
		 
		void Bind();
		void Unbind();

		inline uint32_t GetProgramId() { return _RendererId; }

		void UploadUniformMat4(const glm::mat4& mat4, const std::string& name);

	private:
		uint32_t _RendererId;
	};
}