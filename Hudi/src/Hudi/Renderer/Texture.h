#pragma once

#include "Hudi/Core/Core.h"

namespace Hudi {
	
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, size_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool IsValid() const = 0;
		virtual bool operator== (const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::filesystem::path& filePath);

	public:
		glm::vec2 lowerBound = { 0.0f, 0.0f };
		glm::vec2 upperBound = { 1.0f, 1.0f };
	};
}