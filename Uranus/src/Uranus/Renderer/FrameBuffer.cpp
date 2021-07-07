#include "urpch.h"
#include "FrameBuffer.h"
#include "Uranus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Uranus {

	Ref<FrameBuffer> Uranus::FrameBuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetApi())
		{
		case RendererAPI::API::None:
			UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}
