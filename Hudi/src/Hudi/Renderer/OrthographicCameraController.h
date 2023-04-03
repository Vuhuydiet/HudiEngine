#pragma once

#include "Hudi/Events/Event.h"
#include "OrthographicCamera.h"

#include <glm/glm.hpp>

namespace Hudi {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotate = false);

		void OnUpdate(float dt);
		void OnEvent(Event& e);

		void OnResize(float width, float heigth);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		float GetZooomLevel() const { return m_ZoomLevel; }
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		OrthographicCamera m_Camera;

		bool m_IsRotate;
		glm::vec3 m_CameraPosition;
		float m_CameraRotation;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
	};

}