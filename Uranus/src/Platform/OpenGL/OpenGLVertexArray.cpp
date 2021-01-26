#include "urpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Uranus {

	static GLenum ShaderDateTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case Uranus::ShaderDataType::Float:		return GL_FLOAT;
		case Uranus::ShaderDataType::Float2:	return GL_FLOAT;
		case Uranus::ShaderDataType::Float3:	return GL_FLOAT;
		case Uranus::ShaderDataType::Float4:	return GL_FLOAT;
		case Uranus::ShaderDataType::Mat3:		return GL_FLOAT;
		case Uranus::ShaderDataType::Mat4:		return GL_FLOAT;
		case Uranus::ShaderDataType::Int:		return GL_INT;
		case Uranus::ShaderDataType::Int2:		return GL_INT;
		case Uranus::ShaderDataType::Int3:		return GL_INT;
		case Uranus::ShaderDataType::Int4:		return GL_INT;
		case Uranus::ShaderDataType::Bool:		return GL_BOOL;
		}

		UR_CORE_ASSERT(false, "Unknown Shader Data Type!")
			return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		UR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

		glBindVertexArray(_RendererId);
		vertexBuffer->Bind();

		uint8_t index = 0;
		BufferLayout& bufferLayout = vertexBuffer->GetLayout();

		for (auto& element : bufferLayout.GetElements()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, GetShderTypeComponentCount(element.Type),
				ShaderDateTypeToOpenGLBaseType(element.Type), element.Normlized ? GL_TRUE : GL_FALSE,
				bufferLayout.GetStride(), (const void*)element.Offset);
			index++;
		}

		_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		_IndexBuffer = indexBuffer;
		indexBuffer->Bind();
	}
}
