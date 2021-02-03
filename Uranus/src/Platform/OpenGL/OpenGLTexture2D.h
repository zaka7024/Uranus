#pragma once
#include "Uranus/Renderer/Texture.h"

namespace Uranus {

	class OpenGLTexture2D : public Texture2D {

	public:
		OpenGLTexture2D(const std::string& path);
		virtual void Bind(uint32_t slot = 0) override;
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const { return _Width; }
		virtual uint32_t GetHeight() const { return _Height; }

	private:
		std::string _TexturPath;
		uint32_t _Width, _Height;
		uint32_t _RendererId;
	};
}