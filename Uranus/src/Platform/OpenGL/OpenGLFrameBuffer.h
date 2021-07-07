#pragma once
#include "Uranus/Renderer/FrameBuffer.h"

namespace Uranus {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		~OpenGLFrameBuffer();
		virtual void Bind() override;
		virtual void Ubnind() override;

		virtual uint32_t GetColorAttachmentRendererId() { return _ColorAttachment; };

		virtual const FramebufferSpecification& GetSpecification() { return _Specification; };
	private:
		uint32_t _RendererId;
		uint32_t _ColorAttachment, _DepthAttachment;
		FramebufferSpecification _Specification;
	};
}