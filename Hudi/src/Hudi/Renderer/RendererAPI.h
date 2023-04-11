#pragma once

#include <glm/glm.hpp>

#include "Hudi/Core/Core.h"

#include "VertexArray.h"

namespace Hudi {

	class RendererAPI
	{
	public:
		enum class API { None, OpenGL };
	public:
		virtual ~RendererAPI() = default;
		virtual void Init() = 0;
		virtual void SetViewport(int x, int y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(Ref<VertexArray> vertexArrray) = 0;
		virtual void DrawIndexed(Ref<VertexArray> vertexArray, size_t count) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}