#pragma once

#include <ECS.h>

namespace Hudi {

	struct SpriteRenderer : public ECS::Component
	{
		SpriteRenderer()
			: order(0)
		{
		}
		~SpriteRenderer()
		{
		}
		const char* ToString() const override { return "SpriteRenderer"; }

		void SetFile(const char* image)
		{
		}

		void SetSourceRect(int x, int y, int w, int h)
		{
		}

		const char* GetFilePath() { return m_FilePath; }

	public:
		uint32_t order;

	private:
		int* m_Texture = nullptr;
		int m_SrcRect;
		const char* m_FilePath = nullptr;

	private:
		friend class RenderSystem;
	};

}