#pragma once

#include <string>

namespace Uranus {

	class Shader {
	public:
		Shader(std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();
		 
		void Bind();
		void Unbind();

	private:
		uint32_t _RendererId;
	};
}