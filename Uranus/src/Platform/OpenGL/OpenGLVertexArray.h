#pragma once
#include "Uranus/Renderer/VertexArray.h"
#include <memory>
#include <vector>

namespace Uranus {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

		inline virtual IndexBuffer& GetIndexBuffer() const override { return *_IndexBuffer; };

	private:
		uint32_t _RendererId;
		std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
		std::shared_ptr<IndexBuffer> _IndexBuffer;
	};
}
