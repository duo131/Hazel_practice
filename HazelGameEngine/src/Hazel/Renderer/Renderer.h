#pragma once

namespace Hazel {

	enum class RendererAPI
	{
		NONE = 0, OPENGL = 1, DIRECTX = 2
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetRedererAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
	
}