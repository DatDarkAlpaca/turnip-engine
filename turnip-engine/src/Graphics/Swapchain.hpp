#pragma once
#include <glm/glm.hpp>
#include "Graphics/Type/Rect.hpp"

namespace tur
{
	struct SwapchainDescriptor
	{
		Rect2D resolution;
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
		explicit ISwapchain(const SwapchainDescriptor& descriptor)
			: m_Descriptor(descriptor)
		{

		}

		virtual ~ISwapchain() = default;

	public:
		virtual void Present() = 0;


	public:
		SwapchainDescriptor& GetDescriptor() { return m_Descriptor; }

		Rect2D GetResolution() { return m_Descriptor.resolution; }

	private:
		SwapchainDescriptor m_Descriptor;
	};
}