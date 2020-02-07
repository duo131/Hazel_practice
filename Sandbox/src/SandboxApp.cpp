#include <Hazel.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExamplePlayer : public  Hazel::Layer
{
public:
	ExamplePlayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) , m_CameraPosition(0.0f, 0.0f, 0.0f), m_SquarePosition(0.0f)
	{

		m_VertexArray.reset(Hazel::VertexArray::Create());

		// OpenGl without transform so only in projection space [-1, 1] for x, y, z as NDR 
		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f,  /*color*/ 0.7f, 0.3f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f,  /*color*/ 0.2f, 0.4f, 0.9f, 1.0f,
			 0.0f,  0.5f,  0.0f,  /*color*/ 0.8f, 0.7f, 0.2f, 1.0f
		};


		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		//Define the layout
		Hazel::BufferLayout layout = {
			{Hazel::ShaderDataType::FLOAT3, "a_Position" },
			{Hazel::ShaderDataType::FLOAT4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		uint32_t indices[3] = { 0, 1 ,2 };
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVertexArray.reset(Hazel::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f,  1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));


		squareVertexBuffer->SetLayout({
			{Hazel::ShaderDataType::FLOAT3, "a_Position" },
			{Hazel::ShaderDataType::FLOAT2, "a_TexCoord" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Hazel::IndexBuffer::Create(squareIndices, 6));

		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);


		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;	

			uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position , 1.0);
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

		m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		//flatColorShader
		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			void main()
			{
				a_Color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader2 = Hazel::Shader::Create("FlatColor", vertexSrc2, fragmentSrc2);
	

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader> (textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader> (textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
	
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * timestep;

		//
		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * timestep;
		//
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL))
		{
			m_CameraPosition = { 0.0f,0.0f,0.0f };
			m_SquarePosition = { 0.0f,0.0f,0.0f };
			m_CameraRotation = 0.0f;
		}
		Hazel::RenderCommand::SetColorClear(glm::vec4(0.2f, 0.2f, 0.2f, 1));
		Hazel::RenderCommand::Clear();
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		Hazel::Renderer::BeginScene(m_Camera);
		//Hazel::Renderer::BeginScene(m_Scene);
		//Hazel::Renderer2D::BeginScene(m_Camera);
		//Hazel::Renderer2D::DrawQuad();

		//bind data to render
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		
		//glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		//glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_Shader2)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_Shader2)->UploadUniformFloat3("u_Color", m_SquareColor);
		
		//draw squre 
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				//if (i % 2 == 0)
				//	m_Shader2->UploadUniformFloat4("u_Color", redColor);
				//else
				//	m_Shader2->UploadUniformFloat4("u_Color", blueColor);
				Hazel::Renderer::Submit(m_Shader2, m_SquareVertexArray, transform);
			}
			
		}
		// texture square

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		
		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//draw triangle 
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();


		//Renderer::Flush();

	}

	virtual void OnImGuiRender() override
	{
		//Adding ImGui UI control
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//Hazel::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExamplePlayer::OnKeyPressedEvent));
	}

	// using this is like a key event, the camera wont move smooth
	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == HZ_KEY_LEFT)
			m_CameraPosition.x -= m_CameraMoveSpeed;
		if (event.GetKeyCode() == HZ_KEY_RIGHT)
			m_CameraPosition.x += m_CameraMoveSpeed;
		if (event.GetKeyCode() == HZ_KEY_UP)
			m_CameraPosition.y += m_CameraMoveSpeed;
		if (event.GetKeyCode() == HZ_KEY_DOWN)
			m_CameraPosition.y -= m_CameraMoveSpeed;

		return false;
	}
private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref <Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_Shader2;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 100.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

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
