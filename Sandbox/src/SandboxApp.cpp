#include <Hazel.h>

#include "imgui/imgui.h"

class ExamplePlayer : public  Hazel::Layer
{
public:
	ExamplePlayer() : Layer("Example") {}

	void OnUpdate() override
	{
		//HZ_INFO("ExamplePlayer::Update");

		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("TAB keypressed(poll)");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//HZ_TRACE("{0}", event);
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
				HZ_TRACE("TAB keypressed(event)");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};
class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExamplePlayer());
		//PushOverlay(new Hazel::ImGuiLayer());
	}
	~Sandbox()
	{}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
