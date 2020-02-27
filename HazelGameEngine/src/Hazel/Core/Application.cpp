#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/Log.h"

#include "Input.h"
#include "Hazel/Renderer/Renderer.h"


#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	//assign singleton(only one application a time) by this as pointer for other use
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		//m_Window->SetVSync(false);

		//Renderer
		Renderer::Init();


		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}


	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		//HZ_CORE_TRACE("{0}",e);

		//Event layer: from top layer to bottom layer(backward)
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{

		while (m_Running)
		{	
			float time = (float)glfwGetTime(); // should be platform getTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;


			if (!m_Minimized) // if minimized, no need to update
			{
				// render layer: from bottom layer to top layer
				for (auto layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
		
			}
			// for ImGui Layer
			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			//auto [x, y] = Input::GetMousePostion();
			//HZ_CORE_TRACE("{0}, {1}", x, y);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		//when windows minimize, the height and width will be 0
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::WindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}