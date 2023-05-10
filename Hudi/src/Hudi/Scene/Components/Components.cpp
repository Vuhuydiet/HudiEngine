#include "hdpch.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

namespace Hudi {

	Transform::Transform() : scale(1.0f), localScale(1.0f) {}

	void Transform::Translate(float x, float y, float z) { position += Vec3(x, y, z); }
	void Transform::Translate(Vec3 v) { position += v; }

	glm::mat4 Transform::Transformation() const
	{
		return glm::translate(glm::mat4(1.0f), (glm::vec3)position) *
			glm::toMat4(glm::quat(glm::radians((glm::vec3)rotation))) *
			glm::scale(glm::mat4(1.0f), (glm::vec3)scale);
	}

	void Camera::SetProjectionData(const OrthographicData& data)
	{
		if (m_OrthographicData == data)
			return;
		m_OrthographicData = data;
	}

	void Camera::SetProjectionData(const PerspectiveData& data)
	{
		if (m_Perspective == data)
			return;
		m_Perspective = data;
	}

	void Camera::Resize(float width, float height)
	{
		m_OrthographicData = { -width * internalRatio * 0.5f, width * internalRatio * 0.5f, -height * internalRatio * 0.5f, height * internalRatio * 0.5f };
		m_Perspective.aspectRatio = width / height;
	}

	void Camera::Resize(const glm::vec2& size)
	{
		Resize(size.x, size.y);
	}

	void Camera::RecalculateProjection()
	{
		if (m_Type == Orthographic)
		{
			const auto& tc = GetComponent<Transform>();
			float Near = tc.position.z;
			float Far = tc.position.z + (m_OrthographicData.zFar - m_OrthographicData.zNear);
			//m_Projection = glm::ortho(m_OrthographicData.left, m_OrthographicData.right, 
			//	m_OrthographicData.bottom, m_OrthographicData.top, Near, Far);
			m_Projection = glm::ortho(m_OrthographicData.left, m_OrthographicData.right,
				m_OrthographicData.bottom, m_OrthographicData.top, m_OrthographicData.zNear, m_OrthographicData.zFar);
		}
		else if (m_Type == Perspective)
		{
			m_Projection = glm::perspective(glm::radians(m_Perspective.FOV), m_Perspective.aspectRatio, m_Perspective.zNear, m_Perspective.zFar);
		}
	}


}