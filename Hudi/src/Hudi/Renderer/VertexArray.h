#pragma once

#include "Hudi/Core/Core.h"
#include "Buffer.h"

namespace Hudi {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;

		virtual Ref<VertexBuffer> GetVertexBuffer() const = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}