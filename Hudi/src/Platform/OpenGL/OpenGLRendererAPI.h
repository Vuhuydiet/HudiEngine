#pragma once

#include "Hudi/Renderer/RendererAPI.h"

namespace Hudi {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void EnableDepthTest(bool enable) override;
		virtual void SetViewport(int x, int y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(Ref<VertexArray> vertexArray) override;
		virtual void DrawIndexed(Ref<VertexArray> vertexArray, size_t count) override;

	};

}