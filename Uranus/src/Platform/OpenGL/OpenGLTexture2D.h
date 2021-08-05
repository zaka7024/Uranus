#pragma once
#include "Uranus/Renderer/Texture.h"

#include <glad/glad.h>

namespace Uranus {

	class OpenGLTexture2D : public Texture2D {

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);

		virtual void Bind(uint32_t slot = 0) override;
		virtual void SetData(void* data, uint32_t size) override;

		virtual ~OpenGLTexture2D();

		virtual bool operator==(const Texture& other) const override 
		{
			return _RendererId == ((OpenGLTexture2D&)other)._RendererId;
		}

		virtual uint32_t GetWidth() const { return _Width; }
		virtual uint32_t GetHeight() const { return _Height; }
		virtual uint32_t GetRendereId() const { return _RendererId; }

		virtual std::filesystem::path GetAssetFilePath() {
			return std::filesystem::path(_TexturPath);
		}

	private:
		std::string _TexturPath;
		uint32_t _Width, _Height;
		uint32_t _RendererId;
		GLenum _InternalFormat, _DataFormat;
	};
}