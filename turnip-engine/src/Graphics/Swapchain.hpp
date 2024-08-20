#pragma once
#include <glm/glm.hpp>

namespace tur
{
	struct SwapchainDescriptor
	{
		glm::vec2 resolution;
		uint32_t colorBits = 32;
		uint32_t depthBits = 24;
		uint32_t stencilBits = 8;
		uint32_t samples = 1;
		uint32_t swapBuffers = 2;
		bool fullscreen = false;
	};

	class ISwapchain
	{
	public:
		virtual ~ISwapchain() = default;

	public:
		virtual void Present() = 0;
	};
}