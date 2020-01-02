#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {

	//temp function
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::NONE:   break;
		case ShaderDataType::FLOAT:  return GL_FLOAT;
		case ShaderDataType::FLOAT2: return GL_FLOAT;
		case ShaderDataType::FLOAT3: return GL_FLOAT;
		case ShaderDataType::FLOAT4: return GL_FLOAT;
		case ShaderDataType::MAT3:   return GL_FLOAT;
		case ShaderDataType::MAT4:   return GL_FLOAT;
		case ShaderDataType::INT:    return GL_INT;
		case ShaderDataType::INT2:   return GL_INT;
		case ShaderDataType::INT3:   return GL_INT;
		case ShaderDataType::INT4:   return GL_INT;
		case ShaderDataType::BOOL:   return GL_BOOL;
		default:                     break;
		}

		HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLaypout().GetElements().size(), "VertexBuffer has no layout");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vertexBuffer->GetLaypout())
		{
			glEnableVertexAttribArray(index);
			// no normalize
			glVertexAttribPointer(index,
				element.GetElementCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLaypout().GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}