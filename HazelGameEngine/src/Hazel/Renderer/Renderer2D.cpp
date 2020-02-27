#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel 
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		//Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_RendererData;
	

	void Renderer2D::Init()
	{
		s_RendererData = new Renderer2DStorage();

		s_RendererData->QuadVertexArray = VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));


		squareVertexBuffer->SetLayout({
			{ShaderDataType::FLOAT3, "a_Position" },
			{ShaderDataType::FLOAT2, "a_TexCoord" },
			});
		s_RendererData->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, 6));

		s_RendererData->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		//s_RendererData->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_RendererData->TextureShader = Shader::Create("assets/shaders/ColorTexture.glsl");
		s_RendererData->TextureShader->Bind();
		s_RendererData->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::ShutDown()
	{
		delete s_RendererData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_RendererData->TextureShader->Bind();
		s_RendererData->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_RendererData->TextureShader->SetFloat4("u_Color", color);
		// Bind Tecture

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_RendererData->TextureShader->SetMat4("u_Transform", transform);

		s_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_RendererData->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_RendererData->TextureShader->SetMat4("u_Transform", transform);

		s_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_RendererData->QuadVertexArray);
	}

}