#pragma once

#include "Hudi/Core/Core.h"

namespace Hudi {

	enum class FramebufferTextureFormat
	{
		None = 0,

		RGBA8,
		RED_INT,

		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpec
	{
		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(FramebufferTextureFormat format)
			: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpec
	{
		FramebufferAttachmentSpec() = default;
		FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> _attachments)
			: attachments(_attachments) {}
		std::vector<FramebufferTextureSpec> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height;
		FramebufferAttachmentSpec attachments;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(int width, int height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specs);
	};

}