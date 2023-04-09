#pragma once

#include "Hudi/Core/Core.h"

#include "RendererAPI.h"
#include "RenderCommand.h"

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

#include "OrthographicCamera.h"

#include <glm/glm.hpp>

namespace Hudi {

	struct Quad
	{
		glm::vec3 position;
		glm::vec2 size;
		float angle;
		Ref<Texture2D> texture;
		glm::vec4 color;
		float tilingFactor;

		Quad()
			: position(glm::vec3(0.0f)), size(glm::vec2(1.0f)), angle(0.0f), texture(nullptr), color(glm::vec4(1.0f)), tilingFactor(1.0f) {}
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		
		static void Flush();

		static void DrawQuad(const Quad& quad);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture);

	private:
		static void BeginNewBatch();
		static void Reset();
	};

}