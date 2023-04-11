#pragma once

#include "Hudi/Renderer/Framebuffer.h"

namespace Hudi {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specs);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecifucation() const override { return m_Specification; }

	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
	};

}