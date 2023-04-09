#pragma once

#include "Hudi/Core/Core.h"

#include "RendererAPI.h"
#include "RenderCommand.h"

#include "Shader.h"
#include "VertexArray.h"

#include "OrthographicCamera.h"

namespace Hudi {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& tranform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 projectionViewMatrix;
		};
		static SceneData m_Data;

	};

}