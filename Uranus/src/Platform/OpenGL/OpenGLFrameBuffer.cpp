#include "urpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Uranus {

	static const uint32_t MaxFrameBufferSize = 8192;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec)
		:_Specification(spec)
	{
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate() {
		
		if (_RendererId) {
			glDeleteFramebuffers(1, &_RendererId);
			glDeleteTextures(1, &_ColorAttachment);
			glDeleteTextures(1, &_DepthAttachment);
		}

		glCreateFramebuffers(1, &_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, _RendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, _ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Specification.Width, _Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

		UR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &_RendererId);
		glDeleteTextures(1, &_ColorAttachment);
		glDeleteTextures(1, &_DepthAttachment);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _RendererId);
		glViewport(0, 0, _Specification.Width, _Specification.Height);
	}

	void OpenGLFrameBuffer::Ubnind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MaxFrameBufferSize || height > MaxFrameBufferSize) {
			UR_CORE_INFO("Trying to resize frmebuffer to width: {0}, height: {1}", width, height);
			return;
		}
		_Specification.Width = width;
		_Specification.Height = height;
		
		Invalidate();
	}
}
