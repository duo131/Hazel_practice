#include "hzpch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetRedererAPI())
		{
			case RendererAPI::API::NONE:
				HZ_CORE_ASSERT(false, "RendererAPI not support");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return new OpenGLShader(vertexSrc, fragmentSrc);
			default:
				break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}
}