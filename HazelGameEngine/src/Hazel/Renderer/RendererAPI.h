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
		virtual void SetColorClear(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return  s_API; }
	private:
		static API s_API;

	};
}