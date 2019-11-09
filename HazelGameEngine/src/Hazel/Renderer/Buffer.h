#pragma once

namespace Hazel {

	enum class ShaderDataType: uint8_t
	{
		NONE = 0, 
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MAT3, MAT4,
		INT, INT2, INT3, INT4,
		BOOL
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::NONE:   break;
		case ShaderDataType::FLOAT:  return 4;
		case ShaderDataType::FLOAT2: return 4 * 2;
		case ShaderDataType::FLOAT3: return 4 * 3;
		case ShaderDataType::FLOAT4: return 4 * 4;
		case ShaderDataType::MAT3:   return 4 * 3 * 3;
		case ShaderDataType::MAT4:   return 4 * 4 * 4;
		case ShaderDataType::INT:    return 4;
		case ShaderDataType::INT2:   return 4 * 2;
		case ShaderDataType::INT3:   return 4 * 3;
		case ShaderDataType::INT4:   return 4 * 4;
		case ShaderDataType::BOOL:   return 1;
		default:                     break;
		}

		HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}
		BufferElement( ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}

		uint32_t GetElementCount() const
		{
			switch (Type)
			{
			case ShaderDataType::NONE:   break;
			case ShaderDataType::FLOAT:  return 1;
			case ShaderDataType::FLOAT2: return 2;
			case ShaderDataType::FLOAT3: return 3;
			case ShaderDataType::FLOAT4: return 4;
			case ShaderDataType::MAT3:   return 3 * 3;
			case ShaderDataType::MAT4:   return 4 * 4;
			case ShaderDataType::INT:    return 1;
			case ShaderDataType::INT2:   return 2;
			case ShaderDataType::INT3:   return 3;
			case ShaderDataType::INT4:   return 4;
			case ShaderDataType::BOOL:   return 1;
			default:                     break;
			}

			HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
			return 0;
		}
		
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements) 
		{
			CaculateOffsetsAndStrides();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }


		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CaculateOffsetsAndStrides()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const BufferLayout GetLaypout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer 
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;
		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}