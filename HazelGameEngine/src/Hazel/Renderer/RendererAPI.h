#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Hazel {

	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0, OPENGL = 1, DIRECTX = 2
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetColorClear(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return  s_API; }
	private:
		static API s_API;

	};
}