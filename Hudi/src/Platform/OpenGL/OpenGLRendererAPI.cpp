#include "hdpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Hudi {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}


	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(Ref<VertexArray> vertexArray)
	{
		glDrawElements(GL_TRIANGLES, (uint32_t)vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}