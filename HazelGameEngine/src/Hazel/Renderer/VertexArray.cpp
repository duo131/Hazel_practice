#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRedererAPI())
		{
		case RendererAPI::NONE:
			HZ_CORE_ASSERT(false, "RendererAPI not support");
			return nullptr;
		case RendererAPI::OPENGL:
			return new OpenGLVertexArray();
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}
}