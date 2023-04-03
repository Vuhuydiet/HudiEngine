#pragma once

#include "Hudi/Core/Core.h"

#include "RendererAPI.h"
#include "RenderCommand.h"

#include "Shader.h"
#include "VertexArray.h"

#include "OrthographicCamera.h"

namespace Hudi {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	};

}