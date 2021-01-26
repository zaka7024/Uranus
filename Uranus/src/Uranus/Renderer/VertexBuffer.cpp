#include "urpch.h"
#include "VertexArray.h"

#include "Uranus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Uranus {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetApi())
		{
		case RendererAPI::None:
			UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}