#pragma once
#include "Core.h"

namespace Hazel {
	class HZ_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();
}

