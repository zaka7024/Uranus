#include "urpch.h"
#include "Texture.h"

#include "Uranus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Uranus {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetApi())
		{
		case RendererAPI::API::None:
			UR_CORE_ASSERT(false, "RendererAPI::None in currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		UR_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}