#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace Hazel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotatio = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		void ResetCamera();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		// m_AspectRatio and m_ZoomLevel have to initialize before the camera, so order is matter
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f };
		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 100.0f;
	};
}