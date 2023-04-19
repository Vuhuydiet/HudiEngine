#pragma once

#include "Component.h"

#include "Hudi/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Hudi {

	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer()
			: filepath(""), texture(nullptr), color(1.0f), size(1.0f), order(0)
		{
		}
		const char* ToString() const override { return "SpriteRenderer"; }

		void SetFilePath(const std::string& _filepath)
		{
			if (_filepath.empty())
			{
				texture = nullptr;
				filepath = "";
				return;
			}

			Ref<Texture2D> tex = Texture2D::Create(_filepath);
			if (tex->Valid())
			{
				texture = tex;
				filepath = _filepath;
			}
		}

		void SetSourceRect(int x, int y, int w, int h)
		{
		}

	public:
		std::string filepath;

		Ref<Texture2D> texture;
		glm::vec4 color;
		glm::vec2 size;

		uint32_t order;
		
	};

}