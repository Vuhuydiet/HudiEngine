#include "hdpch.h"
#include "OrthographicCameraController.h"

#include "Hudi/Core/Input.h"
#include "Hudi/Core/Application.h"

namespace Hudi {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotate)
		: m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f), 
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_IsRotate(rotate),
		m_CameraPosition(0.0f),
		m_CameraRotation(0.0f)
	{}

	void OrthographicCameraController::OnUpdate(float dt)
	{
		if (Input::IsKeyDown(Key::A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
		}
		if (Input::IsKeyDown(Key::D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * dt;;
		}
		if (Input::IsKeyDown(Key::W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * dt;
		}
		if (Input::IsKeyDown(Key::S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * dt;;
		}
		m_Camera.SetPosition(m_CameraPosition);

		if (m_IsRotate)
		{
			if (Input::IsKeyDown(Key::F))
			{
				m_CameraRotation += m_CameraRotationSpeed * dt;
			}
			if (Input::IsKeyDown(Key::G))
			{
				m_CameraRotation -= m_CameraRotationSpeed * dt;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		if (width <= 0.001f || height <= 0.001f)
		{
			HD_CORE_ERROR("Trying to resize the camera to an invalid value: {0}, {1}", width, height);
			return;
		}
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		SDL_Event event = (SDL_Event)e;
		switch (event.type)
		{
		case MOUSE_WHEEL:
		{
			m_ZoomLevel -= event.wheel.y * 0.1f;
			m_ZoomLevel = std::min(m_ZoomLevel, 10.0f);
			m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);

			m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			break;
		}
		case WINDOW_EVENT:
			if (event.window.event == WINDOWEVENT_RESIZED)
			{
				auto& app = Application::Get();
				OnResize((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());
			}
			break;
		}
	}

}