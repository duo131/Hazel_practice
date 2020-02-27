#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(2780.0f / 1620.0f)
{
}

void Sandbox2D::OnAttach()
{

	m_CheckBoardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Hazel::RenderCommand::SetColorClear(glm::vec4(0.2f, 0.2f, 0.2f, 1));
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.6f, 0.6f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 3.8f, 3.8f }, m_CheckBoardTexture);
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	//Adding ImGui UI control
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
