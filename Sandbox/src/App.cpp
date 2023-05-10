#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "EditorLayer.h"
#include "Sandbox2D.h"

namespace Hudi {

	class ExampleLayer : public Hudi::Layer
	{
	public:
		ExampleLayer()
			: m_CameraController(800.0f / 600.0f, true), m_Color(1.0f)
		{
			m_VertexArray = VertexArray::Create();
			m_VertexArray->Bind();

			float vertices[4 * 5] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
			vertexBuffer->SetLayout({
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float2, "a_TextCoord"}
				});
			m_VertexArray->SetVertexBuffer(vertexBuffer);

			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 6);
			m_VertexArray->SetIndexBuffer(indexBuffer);

			std::string vertexSource = R"(
				#version 450 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec2 a_TexCoord;

				uniform mat4 u_ProjectionViewMatrix;
				uniform mat4 u_Transform;

				out vec2 v_TexCoord;
				void main()
				{
					v_TexCoord = a_TexCoord;
					gl_Position = u_ProjectionViewMatrix * u_Transform * vec4(a_Position, 1.0);
				}
			)";

			std::string fragmentSource = R"(
				#version 450 core

				out vec4 color;

				in vec2 v_TexCoord;

				uniform sampler2D u_Texture;
				uniform sampler2D u_Texture1;

				void main()
				{
					color = mix(texture(u_Texture, v_TexCoord), texture(u_Texture1, v_TexCoord), 0.3);
				}
			)";

			m_Shaders.Load("TextureShader", vertexSource, fragmentSource);
			m_Shaders.Load("assets/shaders/Texture.glsl");
			auto shader = m_Shaders.Get("Texture");

			m_Texture = Texture2D::Create("assets/textures/hz.png");
			m_Texture1 = Texture2D::Create("assets/textures/pepefrog.jpg");

			shader->SetUniform("u_Texture", 0);
			shader->SetUniform("u_Texture1", 1);
		}
		
		virtual void OnUpdate(float dt) override
		{
			m_CameraController.OnUpdate(dt);

			RenderCommand::SetClearColor({ 0.8f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene(m_CameraController.GetCamera());
			{
				m_Texture->Bind(0);
				m_Texture1->Bind(1);
				Renderer::Submit(m_Shaders.Get("Texture"), m_VertexArray);

				Renderer::EndScene();
			}
		}

		virtual void OnEvent(Event& e) override
		{
			m_CameraController.OnEvent(e);
		}

		virtual void OnImGuiRender() override
		{
			/*ImGui::Begin("Color Settings");

			ImGui::ColorEdit3("Color: ", glm::value_ptr(m_Color));

			ImGui::End();*/
		}

	private:

		ShaderLibrary m_Shaders;

		Ref<VertexArray> m_VertexArray;

		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_Texture1;

		OrthographicCameraController m_CameraController;

		glm::vec3 m_Color;

	};

}

class Sandbox : public Hudi::Application
{
public:
	Sandbox()
	{
		//Hudi::Time::SetFPS(30);

		//PushLayer(new Hudi::EditorLayer());
		//PushLayer(new Hudi::ExampleLayer());
		PushLayer(new Hudi::Sandbox2D());

		////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		Hudi::SceneManager::Get().CreateNewScene("Default");
		/*auto& scene = Hudi::SceneManager::Get().GetActiveScene();
		scene.CreateGameObject("vu dep trai");
		scene.CreateGameObject("trai");

		auto p_vu = scene.GetGameObjectByRef("vu dep trai");
		auto p_v = scene.GetGameObjectByRef("trai");
		p_vu->AddChild(p_v);
		p_v->SetParent(p_vu);*/
	}

	~Sandbox()
	{
	}

private:

};

Hudi::Application* Hudi::CreateApplication()
{
	return new Sandbox();
}
