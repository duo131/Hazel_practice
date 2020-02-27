#pragma once
#include <Hazel.h>

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	//temp
	Hazel::Ref <Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Texture2D> m_CheckBoardTexture;
	
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

};