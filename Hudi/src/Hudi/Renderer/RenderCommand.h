#pragma once

#include "RendererAPI.h"

namespace Hudi {

	class RenderCommand
	{
	public:
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetViewport(int x, int y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }

		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		inline static void DrawIndexed(Ref<VertexArray> vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
		inline static void DrawIndexed(Ref<VertexArray> vertexArray, size_t count) { s_RendererAPI->DrawIndexed(vertexArray, count); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}