#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel 
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "HazelEngine",
			const unsigned int width = 2780, const unsigned int height = 1620)
			:Title(title), Width(width), Height(height)
		{}
	};

	// Interface representing a desktop system based Window
	class HZ_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// return the GLFW(or other) window. This is for other binary/function to get window 
		virtual void* GetNativeWindow() const = 0;

		//has to implement for each platform
		static Window* Create(const WindowProps& props = WindowProps());

	};
}