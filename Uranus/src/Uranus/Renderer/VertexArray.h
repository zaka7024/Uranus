#pragma once

#include "Uranus/Renderer/Buffer.h"

namespace Uranus {

	class VertexArray {

	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddVertexBuffer(const Uranus::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Uranus::Ref<IndexBuffer> &indexBuffer) = 0;

		inline virtual IndexBuffer& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
