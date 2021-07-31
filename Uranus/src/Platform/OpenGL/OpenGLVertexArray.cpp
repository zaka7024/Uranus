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
		UR_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		UR_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		UR_PROFILE_FUNCTION();

		glBindVertexArray(_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		UR_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		UR_PROFILE_FUNCTION();

		UR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

		glBindVertexArray(_RendererId);
		vertexBuffer->Bind();

		uint8_t index = 0;
		BufferLayout& bufferLayout = vertexBuffer->GetLayout();

		for (auto& element : bufferLayout.GetElements()) {
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					GetShderTypeComponentCount(element.Type),
					ShaderDateTypeToOpenGLBaseType(element.Type),
					element.Normlized ? GL_TRUE : GL_FALSE,
					bufferLayout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					GetShderTypeComponentCount(element.Type),
					ShaderDateTypeToOpenGLBaseType(element.Type),
					bufferLayout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = GetShderTypeComponentCount(element.Type);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						ShaderDateTypeToOpenGLBaseType(element.Type),
						element.Normlized ? GL_TRUE : GL_FALSE,
						bufferLayout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				UR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}

		}

		_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		UR_PROFILE_FUNCTION();

		_IndexBuffer = indexBuffer;
		indexBuffer->Bind();
	}
}
