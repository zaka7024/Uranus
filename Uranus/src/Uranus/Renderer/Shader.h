#pragma once

#include<unordered_map>
#include <string>

namespace Uranus {

	class Shader {
	public:
		virtual ~Shader() = default;
		 
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual const std::string& GetName() const = 0;

		inline uint32_t GetProgramId() { return _RendererId; }

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filepath);
	private:
		uint32_t _RendererId;
	};

	class ShaderLibrary {
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

		private:
			std::unordered_map<std::string, Ref<Shader>> _Shaders;
	};
}