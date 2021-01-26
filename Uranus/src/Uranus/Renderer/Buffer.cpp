#include "urpch.h"
#include "Buffer.h"

#include "Uranus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Uranus {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetApi())
		{
		case RendererAPI::None:
				UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indecies, uint32_t size) {
		switch (Renderer::GetApi())
		{
		case RendererAPI::None:
			UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indecies, size);
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}
