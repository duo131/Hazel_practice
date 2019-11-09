#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetRedererAPI())
		{
		case RendererAPI::NONE: 
			HZ_CORE_ASSERT(false, "RendererAPI not support");
			return nullptr;
		case RendererAPI::OPENGL:
			return new OpenGLVertexBuffer(vertices, size);
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetRedererAPI())
		{
		case RendererAPI::NONE:
			HZ_CORE_ASSERT(false, "RendererAPI not support");
			return nullptr;
		case RendererAPI::OPENGL:
			return new OpenGLIndexBuffer(indices, size);
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}
}