#pragma once

#include "Hudi/Core/Core.h"

namespace Hudi {

	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentID() const = 0;

		virtual const FramebufferSpecification& GetSpecifucation() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specs);
	};

}