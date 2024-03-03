#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

namespace tur::vulkan
{
	class VMABuilder
	{
	public:
		VmaAllocator Build();

	public:
		VMABuilder& SetArguments(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);

	private:
		vk::Instance m_Instance;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_Device;

	private:
		bool m_ArgumentsSet = false;
	};
}