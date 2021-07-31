#pragma once
#include "Uranus/Core/Core.h"

namespace Uranus {


	enum class FramebufferTextureFormat {
		None = 0,
		// Color
		RGBA8,
		RED_INTEGER,
		// Depth/stencil
		DEPTH24STENCIL8,
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;

		FramebufferTextureSpecification(FramebufferTextureFormat foramt)
			: TextureFormat(foramt) {}

		FramebufferTextureFormat TextureFormat;
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		FramebufferAttachmentSpecification Attachments;
		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Ubnind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual uint32_t GetColorAttachmentRendererId(int index = 0) = 0;

		virtual const FramebufferSpecification& GetSpecification() = 0;

		static Ref<FrameBuffer> Create(const FramebufferSpecification& spec);
	};
}