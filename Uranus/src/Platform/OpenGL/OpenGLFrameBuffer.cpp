#include "urpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Uranus {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec)
		:_Specification(spec)
	{
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
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _RendererId);
	}

	void OpenGLFrameBuffer::Ubnind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
