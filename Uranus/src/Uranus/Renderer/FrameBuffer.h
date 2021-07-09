#pragma once
#include "Uranus/Core/Core.h"

namespace Uranus {

	struct FramebufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		// 
		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Ubnind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererId() = 0;

		virtual const FramebufferSpecification& GetSpecification() = 0;

		static Ref<FrameBuffer> Create(const FramebufferSpecification& spec);
	};
}