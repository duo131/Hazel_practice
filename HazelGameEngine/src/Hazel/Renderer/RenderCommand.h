#pragma once
#include "RendererAPI.h"

namespace Hazel {
	class RenderCommand
	{
	public:
		inline static void SetColorClear(const glm::vec4& color) { s_RendererAPI->SetColorClear(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}