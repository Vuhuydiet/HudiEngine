#pragma once

#include "Hudi/Renderer/Texture.h"

#include <glad/glad.h>

namespace Hudi {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const override;
		
		virtual void SetData(void* data, size_t size) override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual bool operator== (const Texture& other) const override;
			
	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;

		std::string m_FilePath;

	};

}