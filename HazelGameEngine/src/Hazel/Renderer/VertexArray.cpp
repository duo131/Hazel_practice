#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetRedererAPI())
		{
		case RendererAPI::API::NONE:
			HZ_CORE_ASSERT(false, "RendererAPI not support");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return std::make_shared<OpenGLVertexArray>();
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}
}