#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "TransformComponent.h"

namespace Hudi {

	class Camera : public Component
	{
	public:
		enum Type { Perspective = 0, Orthographic = 1 };

		struct OrthographicData
		{
			float left = -5.0f, right = 5.0f, bottom = -5.0f, top = 5.0f, zNear = -10.0f, zFar = 10.0f;
			bool operator== (const OrthographicData& other)
			{
				return left == other.left && right == other.right && bottom == other.bottom && top == other.top && zNear == other.zNear && zFar == other.zFar;
			}
		};

		struct PerspectiveData
		{
			float FOV = 45.0f, aspectRatio = 1.2f, zNear = 0.1f, zFar = 1000.0f;
			bool operator== (const PerspectiveData& other)
			{
				return FOV == other.FOV && aspectRatio == other.aspectRatio && zNear == other.zNear && zFar == other.zFar;
			}
		};

	public:
		Camera() : m_Projection(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 10.0f)), m_Type(Type::Orthographic) {}
		virtual const char* ToString() const override { return "Camera"; }

		Type GetType() const { return m_Type; }
		void Switch(Type _type) { m_Type = _type; }

		float GetInternalRatio() const { return internalRatio; }

		const glm::mat4& GetProjection() { RecalculateProjection(); return m_Projection; }
		const OrthographicData& GetOrthographicData() const { return m_OrthographicData; }
		const PerspectiveData& GetPerspectiveData() const { return m_Perspective; }

		void Resize(float width, float height);
		void Resize(const glm::vec2&);
		void SetProjectionData(const OrthographicData& data);
		void SetProjectionData(const PerspectiveData& data);

		void RecalculateProjection();

	private:
		Type m_Type;
		glm::mat4 m_Projection;

	private:
		const float internalRatio = 1.0f / 100.0f;
		OrthographicData m_OrthographicData;
		PerspectiveData m_Perspective;

	};


	// -------------------------------------------------------------------------- //

	
}