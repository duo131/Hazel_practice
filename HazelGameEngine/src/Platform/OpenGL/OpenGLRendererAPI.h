#pragma once
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

	class OpenGLRenderer : public RendererAPI 
	{
	public:
		virtual void SetColorClear(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}