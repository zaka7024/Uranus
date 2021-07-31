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

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual uint32_t GetColorAttachmentRendererId(int index = 0) { UR_CORE_ASSERT(index < _ColorAttachments.size(), "") return _ColorAttachments[index]; };
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual const FramebufferSpecification& GetSpecification() { return _Specification; };
	private:
		void Invalidate();
		uint32_t _RendererId = 0;
		FramebufferSpecification _Specification;

		std::vector<FramebufferTextureSpecification> _ColorAttachmentSpecifications;
		FramebufferTextureSpecification _DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> _ColorAttachments;
		uint32_t _DepthAttachment = 0;
	};
}