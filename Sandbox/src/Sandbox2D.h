#pragma once

#include <Hudi.h>

namespace Hudi {

	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		~Sandbox2D();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(float dt) override;
		virtual void OnImGuiRender() override;
	private:
		Ref<Texture2D> m_Texture;
		glm::vec4 m_Color;

		OrthographicCameraController m_CameraController;
		
		float frametime = 0.0f;
	};

}