#include "urpch.h"
#include "VertexArray.h"

#include "Uranus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Uranus {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetApi())
		{
		case RendererAPI::API::None:
			UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}