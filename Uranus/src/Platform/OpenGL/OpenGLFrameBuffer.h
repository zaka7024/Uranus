#pragma once
#include "Uranus/Renderer/FrameBuffer.h"

namespace Uranus {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		~OpenGLFrameBuffer();
		virtual void Bind() override;
		virtual void Ubnind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererId() { return _ColorAttachment; };

		virtual const FramebufferSpecification& GetSpecification() { return _Specification; };
	private:
		void Invalidate();
		uint32_t _RendererId = 0;
		uint32_t _ColorAttachment = 0, _DepthAttachment = 0;
		FramebufferSpecification _Specification;
	};
}