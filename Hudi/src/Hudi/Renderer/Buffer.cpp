#include "hdpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Hudi/Core/Core.h"
#include "Renderer.h"

namespace Hudi {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			HD_CORE_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLVertexBuffer>(vertices, size);
		}

		HD_CORE_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

	// ------------------------------------------------------------------ //

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, size_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HD_CORE_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLIndexBuffer>(indices, count);
		}

		HD_CORE_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}


	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.offset = offset;
			offset += element.size;
			m_Stride += element.size;
		}
	}
	uint32_t BufferElement::GetComponentCount() const
	{
		switch (type)
		{
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
		}

		HD_CORE_ERROR("Unknown ShaderDataType!");
		return 0;
	}
}