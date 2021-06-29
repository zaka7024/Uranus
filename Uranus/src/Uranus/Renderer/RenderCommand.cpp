#include "urpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Uranus {

	Scope<RendererAPI> RenderCommand::_RendererAPI = CreateScope<OpenGLRendererAPI>();
}