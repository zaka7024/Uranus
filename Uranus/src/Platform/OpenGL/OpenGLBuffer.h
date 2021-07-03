#pragma once

#include "Uranus/Renderer/Buffer.h"

namespace Uranus {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetLayout(BufferLayout& layout) override;
		virtual BufferLayout& GetLayout() { return _BufferLayout; };

	private:
		uint32_t _RendererId;
		BufferLayout _BufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indieces, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override {
			return _Count;
		}

	private:
		uint32_t _RendererId;
		uint32_t _Count;
	};
}