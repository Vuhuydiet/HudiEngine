#include "hdpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hudi {

#define BOTTOM_LEFT		0
#define BOTTOM_RIGHT	1
#define TOP_RIGHT		2
#define TOP_LEFT		3

	static const uint32_t MAX_QUADS					= 20000;
	static const uint32_t MAX_VERTICES				= MAX_QUADS * 4;
	static const uint32_t MAX_INDICES				= MAX_QUADS * 6;
	static const uint32_t MAX_TEXTURE_SLOT			= 32;

	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
	};

	struct RendererData
	{
		QuadVertex* vertices = nullptr;
		uint32_t quadCount = 0;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOT> textures;
		uint32_t textureCount = 1;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;
	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		s_Data.vertexArray = VertexArray::Create();

		s_Data.vertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex));
		s_Data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"}
		});
		s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);
		s_Data.vertices = new QuadVertex[MAX_VERTICES];

		uint32_t* indices = new uint32_t[MAX_INDICES];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < MAX_INDICES; i += 6)
		{
			indices[i + 0] = offset + BOTTOM_LEFT;
			indices[i + 1] = offset + BOTTOM_RIGHT;
			indices[i + 2] = offset + TOP_RIGHT;

			indices[i + 3] = offset + TOP_RIGHT;
			indices[i + 4] = offset + TOP_LEFT;
			indices[i + 5] = offset + BOTTOM_LEFT;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, MAX_INDICES);
		s_Data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(data));
		whiteTexture->Bind(0);
		s_Data.textures[0] = whiteTexture;

		s_Data.shader = Shader::Create("assets/shaders/Texture.glsl");
		
		int* textureSlots = new int[MAX_TEXTURE_SLOT];
		for (int i = 0; i < MAX_TEXTURE_SLOT; i++)
		{
			textureSlots[i] = i;
		}
		s_Data.shader->Bind();
		s_Data.shader->SetUniform("u_Textures", textureSlots, MAX_TEXTURE_SLOT);
		delete[] textureSlots;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.vertices;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data.shader->Bind();
		s_Data.shader->SetUniform("u_ProjectionView", camera.GetProjectionViewMatrix());
		Reset();
	}

	void Renderer2D::EndScene()
	{
		s_Data.vertexBuffer->SetData(s_Data.vertices, s_Data.quadCount * 4 * sizeof(QuadVertex));
		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 1; i < s_Data.textureCount; i++)
		{
			s_Data.textures[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.vertexArray, s_Data.quadCount * 6);
	}

	void Renderer2D::BeginNewBatch()
	{
		EndScene();
		Reset();
	}

	void Renderer2D::Reset()
	{
		s_Data.quadCount = 0;
		s_Data.textureCount = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		Quad quad;
		quad.position = position;
		quad.size = size;
		quad.color = color;
		DrawQuad(quad);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture)
	{
		Quad quad;
		quad.position = position;
		quad.size = size;
		quad.texture = texture;
		DrawQuad(quad);
	}

	static inline float GetTextureIndex(Ref<Texture2D> texture)
	{
		if (!texture)
			return 0.0f;
		for (uint32_t i = 0; i < s_Data.textureCount; i++)
		{
			if (*s_Data.textures[i].get() == *texture.get())
			{
				return (float)i;
			}
		}
		// TODO: flush texture
		s_Data.textures[s_Data.textureCount++] = texture;
		return (float)s_Data.textureCount - 1.0f;
	}

	void Renderer2D::DrawQuad(const Quad& quad)
	{
		if (s_Data.quadCount >= MAX_QUADS)
		{
			BeginNewBatch();
		}

		float xoffset = quad.size.x / 2.0f;
		float yoffset = quad.size.y / 2.0f;

		float texIndex = GetTextureIndex(quad.texture);

		uint32_t cur_pos = s_Data.quadCount * 4;
		s_Data.vertices[cur_pos + BOTTOM_LEFT].pos = { quad.position.x - xoffset, quad.position.y - yoffset, quad.position.z };
		s_Data.vertices[cur_pos + BOTTOM_LEFT].color = quad.color;
		s_Data.vertices[cur_pos + BOTTOM_LEFT].texCoord = {0.0f, 0.0f};
		s_Data.vertices[cur_pos + BOTTOM_LEFT].texIndex = texIndex;
		s_Data.vertices[cur_pos + BOTTOM_LEFT].tilingFactor = quad.tilingFactor;

		s_Data.vertices[cur_pos + BOTTOM_RIGHT].pos = { quad.position.x + xoffset, quad.position.y - yoffset, quad.position.z };
		s_Data.vertices[cur_pos + BOTTOM_RIGHT].color = quad.color;
		s_Data.vertices[cur_pos + BOTTOM_RIGHT].texCoord = {1.0f, 0.0f};
		s_Data.vertices[cur_pos + BOTTOM_RIGHT].texIndex = texIndex;
		s_Data.vertices[cur_pos + BOTTOM_RIGHT].tilingFactor = quad.tilingFactor;

		s_Data.vertices[cur_pos + TOP_RIGHT].pos = { quad.position.x + xoffset, quad.position.y + yoffset, quad.position.z };
		s_Data.vertices[cur_pos + TOP_RIGHT].color = quad.color;
		s_Data.vertices[cur_pos + TOP_RIGHT].texCoord = {1.0f, 1.0f};
		s_Data.vertices[cur_pos + TOP_RIGHT].texIndex = texIndex;
		s_Data.vertices[cur_pos + TOP_RIGHT].tilingFactor = quad.tilingFactor;

		s_Data.vertices[cur_pos + TOP_LEFT].pos = { quad.position.x - xoffset, quad.position.y + yoffset, quad.position.z };
		s_Data.vertices[cur_pos + TOP_LEFT].color = quad.color;
		s_Data.vertices[cur_pos + TOP_LEFT].texCoord = {0.0f, 1.0f};
		s_Data.vertices[cur_pos + TOP_LEFT].texIndex = texIndex;
		s_Data.vertices[cur_pos + TOP_LEFT].tilingFactor = quad.tilingFactor;

		s_Data.quadCount++;
	}

}