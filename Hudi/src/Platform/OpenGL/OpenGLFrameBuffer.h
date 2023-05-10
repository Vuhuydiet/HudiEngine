#pragma once

#include "Hudi/Renderer/Framebuffer.h"

namespace Hudi {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specs);
		virtual ~OpenGLFramebuffer();

		void Invalidate();
		virtual void Resize(int width, int height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override { HD_CORE_ASSERT_S(index >= m_ColorAttachments.size()); return m_ColorAttachments[index]; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		FramebufferSpecification m_Specification;

		uint32_t m_RendererID = 0;
		std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
		FramebufferTextureSpec m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}