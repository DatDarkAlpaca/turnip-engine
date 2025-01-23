#include "pch.hpp"
#include "vulkan_device.hpp"
#include "vulkan_command_buffer.hpp"
#include "platform/vulkan_context.hpp"

#include "builders/instance_builder.hpp"
#include "builders/physical_device_builder.hpp"
#include "builders/logical_device_builder.hpp"
#include "builders/vma_allocator_builder.hpp"
#include "builders/swapchain_builder.hpp"
#include "builders/frame_data_builder.hpp"

namespace tur::vulkan
{
	void GraphicsDeviceVulkan::initialize_impl(NON_OWNING Window* window, const ConfigData& configData)
	{
		const auto& vulkanConfig = configData.vulkanConfiguration;

		// Instance:
		initialize_instance(m_State, configData);

		// Surface:
		m_State.surface = get_vulkan_surface(m_State.instance, window);

		// Physical Device:
		initialize_physical_device(m_State, vulkanConfig.physicalDeviceRequirements);

		// Logical Device:
		initialize_logical_device(m_State, vulkanConfig);
	
		// VMA Allocator:
		initialize_vma_allocator(m_State);

		// Swapchain:
		initialize_swapchain(m_State, {});

		// Frame Data:
		initialize_frame_data(m_State);
	}

	void GraphicsDeviceVulkan::present_impl()
	{

	}

	CommandBufferVulkan GraphicsDeviceVulkan::create_command_buffer_impl()
	{
		return CommandBufferVulkan(this);
	}
}