#include "hdpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hudi {

	static GLenum SDTtoGLenum(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Bool: return GL_BOOL;
			case ShaderDataType::Int: return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3: return GL_FLOAT;
			case ShaderDataType::Mat4: return GL_FLOAT;
		}

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		if (vertexBuffer->GetLayout().GetElements().size()==0) {
			HD_CORE_ERROR("VertexBuffer's not had any layouts yet. Wrong order?");
			return;
		}

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		BufferLayout layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.GetComponentCount(),
					SDTtoGLenum(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					element.GetComponentCount(),
					SDTtoGLenum(element.type),
					layout.GetStride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						SDTtoGLenum(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				HD_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffer = vertexBuffer;
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
