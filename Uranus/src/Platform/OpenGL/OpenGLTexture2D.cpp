#include "urpch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Uranus {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:_TexturPath(path), _Width(0), _Height(0), _RendererId(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		UR_CORE_ASSERT(data, "Failed to load image");

		_Width = width;
		_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererId);
		glTextureStorage2D(_RendererId, 1, GL_RGBA8, _Width, _Height);
		
		glTextureParameteri(_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_RendererId, 0, 0, 0, _Width, _Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(0 + slot, _RendererId);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_RendererId);
	}
}
