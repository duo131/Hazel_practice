#include "hzpch.h"
#include "Texture.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRedererAPI())
		{
		case RendererAPI::API::NONE:
			HZ_CORE_ASSERT(false, "RendererAPI not support");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return std::make_shared<OpenGLTexture2D>(path);
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "RendererAPI not support");
		return nullptr;
	}

}