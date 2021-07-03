#pragma once
#include <cstdint>

namespace Uranus {

	enum class ShaderDataType {
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case Uranus::ShaderDataType::None:		return 0;
			case Uranus::ShaderDataType::Float:		return 4;
			case Uranus::ShaderDataType::Float2:	return 4 * 2;
			case Uranus::ShaderDataType::Float3:	return 4 * 3;
			case Uranus::ShaderDataType::Float4:	return 4 * 4;
			case Uranus::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case Uranus::ShaderDataType::Mat4:		return 4 * 4 * 4;
			case Uranus::ShaderDataType::Int:		return 4;
			case Uranus::ShaderDataType::Int2:		return 4 * 2;
			case Uranus::ShaderDataType::Int3:		return 4 * 3;
			case Uranus::ShaderDataType::Int4:		return 4 * 4;
			case Uranus::ShaderDataType::Bool:		return 1;
		}

		UR_CORE_ASSERT(false, "Unknown Shader Data Type!")
			return 0;
	}

	static uint32_t GetShderTypeComponentCount(ShaderDataType type) {
		switch (type)
		{
			case Uranus::ShaderDataType::None:		return 0;
			case Uranus::ShaderDataType::Float:		return 1;
			case Uranus::ShaderDataType::Float2:	return 2;
			case Uranus::ShaderDataType::Float3:	return 3;
			case Uranus::ShaderDataType::Float4:	return 4;
			case Uranus::ShaderDataType::Mat3:		return 3 * 3;
			case Uranus::ShaderDataType::Mat4:		return 4 * 4;
			case Uranus::ShaderDataType::Int:		return 1;
			case Uranus::ShaderDataType::Int2:		return 2;
			case Uranus::ShaderDataType::Int3:		return 3;
			case Uranus::ShaderDataType::Int4:		return 4;
			case Uranus::ShaderDataType::Bool:		return 1;
		}

		UR_CORE_ASSERT(false, "Unknown Shader Data Type!")
			return 0;
	}

	struct BufferElement {
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		ShaderDataType Type;
		bool Normlized;

		BufferElement(ShaderDataType type, const std::string& name, bool normlized = false)
			:Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normlized(normlized) {}
	};

	class BufferLayout {
	public:
		BufferLayout()
		:_Stride(0) {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
		:_BufferElements(elements) {

			CalculateOffsetAndStride();

		}

		inline uint32_t GetStride() const { return _Stride; }

	private:
		void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			_Stride = 0;
			for (auto& element : _BufferElements) {
				element.Offset = offset;
				offset += element.Size;
				_Stride += element.Size;
			}
		}

	public:
		const std::vector<BufferElement>& GetElements() { return _BufferElements; }

	private:
		std::vector<BufferElement> _BufferElements;
		uint32_t _Stride;
	};

	class VertexBuffer {

	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(BufferLayout& layout) = 0;
		virtual BufferLayout& GetLayout() = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer {

	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indieces, uint32_t count);
	};
}