#pragma once

#include <string>

namespace Uranus {

	class Shader {
	public:
		virtual ~Shader() = default;
		 
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		inline uint32_t GetProgramId() { return _RendererId; }

		static Shader* Create(std::string& vertexSrc, std::string& fragmentSrc);
	private:
		uint32_t _RendererId;
	};
}