#include "urpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Uranus {

	RendererAPI* RenderCommand::_RendererAPI = new OpenGLRendererAPI();
}