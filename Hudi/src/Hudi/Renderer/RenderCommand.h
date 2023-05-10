#pragma once

#include "RendererAPI.h"

namespace Hudi {

	class RenderCommand
	{
	public:
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetViewport(int x, int y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }

		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void SetClearColor(int r, int g, int b, int a) { s_RendererAPI->SetClearColor({ (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f }); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		inline static void DrawIndexed(Ref<VertexArray> vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
		inline static void DrawIndexed(Ref<VertexArray> vertexArray, size_t count) { s_RendererAPI->DrawIndexed(vertexArray, count); }

	private:
		static RendererAPI* s_RendererAPI;
	};

}