#pragma once

#include <filesystem>

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

		void SetFilePath(const std::filesystem::path& _filepath)
		{
			if (_filepath.empty())
				return;

			Ref<Texture2D> tex = Texture2D::Create(_filepath.string());
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
		std::filesystem::path filepath;

		Ref<Texture2D> texture;
		glm::vec4 color;
		glm::vec2 size;

		uint32_t order;
		
	};

}