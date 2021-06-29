#include "urpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Uranus {

	//////////////////////////////////////////
	// VertexBuffer /////////////////////////
	////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		UR_PROFILE_FUNCTION();

		glGenBuffers(1, &_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		UR_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		UR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		UR_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(BufferLayout& layout)
	{
		_BufferLayout = layout;
	}

	//////////////////////////////////////////
	// IndexBuffer //////////////////////////
	////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indieces, uint32_t count)
		:_Count(count) {

		UR_PROFILE_FUNCTION();

		glGenBuffers(1, &_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indieces, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		UR_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		UR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		UR_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
