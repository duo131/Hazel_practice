#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

#include "Input.h"
#include "Hazel/Renderer/Renderer.h"


#include "glm/glm.hpp"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	//assign singleton(only one application a time) by this as pointer for other use
	Application* Application::s_Instance = nullptr;

	Application::Application(): m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		// OpenGl without transform so only in projection space [-1, 1] for x, y, z as NDR 
		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f,  /*color*/ 0.7f, 0.3f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f,  /*color*/ 0.2f, 0.4f, 0.9f, 1.0f,
			 0.0f,  0.5f,  0.0f,  /*color*/ 0.8f, 0.7f, 0.2f, 1.0f
		};


		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//Define the layout
		BufferLayout layout = {
			{ShaderDataType::FLOAT3, "a_Position" },
			{ShaderDataType::FLOAT4, "a_Color" },
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1 ,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		m_SquareVertexArray.reset(VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f,  0.0f,
			 0.75f, -0.75f,  0.0f,
			 0.75f,  0.75f,  0.0f,
			-0.75f,  0.75f,  0.0f
		};
		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));


		squareVertexBuffer->SetLayout({
			{ShaderDataType::FLOAT3, "a_Position" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer .reset(IndexBuffer::Create(squareIndices, 6));

		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);


		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;	

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position , 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				a_Color = vec4(v_Position, 1.0);
				a_Color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;

			void main()
			{
				a_Color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));
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

			RenderCommand::SetColorClear(glm::vec4(0.2f, 0.2f, 0.2f, 1));
			RenderCommand::Clear();
			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(45.0f);
			Renderer::BeginScene(m_Camera);

			//bind data to render
			//draw squre 
			Renderer::Submit(m_Shader2, m_SquareVertexArray);
			//draw triangle 
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();


			//Renderer::Flush();

			// render layer: from bottom layer to top layer
			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate();
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
}