#pragma once
// same as window.h, this is an interface for input and will be inheritated to other platforms
// but input has to be static

#include "Hazel/Core.h"

namespace Hazel{

	class HZ_API Input 
	{
	public:
		inline static bool IsKeyPressed (int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static std::pair<float, float> GetMousePostion() { return s_Instance->GetMousePostionImpl(); }

	
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePostionImpl() = 0;
	private:
		static Input* s_Instance;
	};
}
