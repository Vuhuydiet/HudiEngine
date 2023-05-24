#include "hdpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Hudi {

	static constexpr int BOT_LEFT = 0;
	static constexpr int BOT_RIGHT = 1;
	static constexpr int TOP_RIGHT = 2;
	static constexpr int TOP_LEFT = 3;

	static constexpr uint32_t MAX_QUADS = 20000;
	static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
	static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
	static constexpr uint32_t MAX_TEXTURE_SLOT = 32;

	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		int texIndex;
		float tilingFactor;
		int quadID;
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
		s_Data.vertices = new QuadVertex[MAX_VERTICES];

		s_Data.vertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex));
		s_Data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_QuadID" }
			});
		s_Data.vertexArray->SetVertexBuffer(s_Data.vertexBuffer);

		uint32_t* indices = new uint32_t[MAX_INDICES];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < MAX_INDICES; i += 6)
		{
			indices[i + 0] = offset + BOT_LEFT;
			indices[i + 1] = offset + BOT_RIGHT;
			indices[i + 2] = offset + TOP_RIGHT;

			indices[i + 3] = offset + TOP_RIGHT;
			indices[i + 4] = offset + TOP_LEFT;
			indices[i + 5] = offset + BOT_LEFT;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, MAX_INDICES);
		s_Data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		whiteTexture->SetData(&data, sizeof(data));
		s_Data.textures[0] = whiteTexture;

		s_Data.shader = Shader::Create("resources/shaders/Texture.glsl");

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

	void Renderer2D::BeginScene(const glm::mat4& cameraProjection, const glm::mat4& cameraTransform)
	{
		s_Data.shader->Bind();
		s_Data.shader->SetUniform("u_ProjectionView", cameraProjection * glm::inverse(cameraTransform));
		Reset();
	}

	void Renderer2D::BeginScene(const glm::mat4& projectionView)
	{
		s_Data.shader->Bind();
		s_Data.shader->SetUniform("u_ProjectionView", projectionView);
		Reset();
	}

	void Renderer2D::EndScene()
	{
		s_Data.vertexBuffer->SetData(s_Data.vertices, s_Data.quadCount * 4 * sizeof(QuadVertex));
		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.textureCount; i++)
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

	static inline int GetTextureIndex(Ref<Texture2D> texture)
	{
		if (!texture)
			return 0;
		for (uint32_t i = 0; i < s_Data.textureCount; i++)
		{
			if (*s_Data.textures[i].get() == *texture.get())
			{
				return i;
			}
		}
		// TODO: flush texture
		s_Data.textures[s_Data.textureCount++] = texture;
		return s_Data.textureCount - 1;
	}

	void Renderer2D::DrawQuad(const Quad& quad, int quadID)
	{
		if (s_Data.quadCount >= MAX_QUADS)
		{
			BeginNewBatch();
		}

		float xoffset = quad.size.x * 0.5f;
		float yoffset = quad.size.y * 0.5f;
		glm::vec2 vertexPos[4] = { {-xoffset, -yoffset}, {xoffset, -yoffset}, {xoffset, yoffset}, {-xoffset, yoffset} };
		glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		int texIndex = GetTextureIndex(quad.texture);

		uint32_t cur_pos = s_Data.quadCount * 4;
		for (int i = 0; i < 4; i++)
		{
			s_Data.vertices[cur_pos + i].pos			= quad.transform * glm::vec4(vertexPos[i], 0.0f, 1.0f);
			s_Data.vertices[cur_pos + i].color			= quad.color;
			s_Data.vertices[cur_pos + i].texCoord		= texCoords[i];
			s_Data.vertices[cur_pos + i].texIndex		= texIndex;
			s_Data.vertices[cur_pos + i].tilingFactor	= quad.tilingFactor;
			s_Data.vertices[cur_pos + i].quadID			= quadID;
		}

		s_Data.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		Quad quad;
		quad.transform = glm::translate(glm::mat4(1.0f), position);
		quad.size = size;
		quad.color = color;
		DrawQuad(quad);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture)
	{
		Quad quad;
		quad.transform = glm::translate(glm::mat4(1.0f), position);
		quad.size = size;
		quad.texture = texture;
		DrawQuad(quad);
	}

}