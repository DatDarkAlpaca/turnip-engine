#include "pch.hpp"
#include "vulkan_gui_system.hpp"
#include "vulkan_device.hpp"
#include "platform/vulkan_context.hpp"

namespace tur::vulkan
{
	VulkanGUI::VulkanGUI(GraphicsDeviceVulkan* device)
		: r_GraphicsDevice(device)
	{
		m_CommandBuffer = tur::make_unique<CommandBufferVulkan>(device->create_command_buffer());
		m_CommandBuffer->initialize();
	}

	void VulkanGUI::initialize_impl()
	{
		initialize_vulkan_gui(r_GraphicsDevice);
	}

	void VulkanGUI::begin_frame_impl()
	{
		begin_vulkan_frame();
	}
	void VulkanGUI::render_impl()
	{
		m_CommandBuffer->begin();
		m_CommandBuffer->begin_render();

		render_vulkan_frame(m_CommandBuffer->get_command_buffer());

		m_CommandBuffer->end_render();
		m_CommandBuffer->end();
		m_CommandBuffer->submit();
	}
	void VulkanGUI::end_frame_impl()
	{
		end_vulkan_frame();
	}
}
