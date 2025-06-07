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
		m_ImguiPool = initialize_vulkan_gui(r_GraphicsDevice);
	}

	void VulkanGUI::shutdown_impl()
	{
		r_GraphicsDevice->wait_idle();
		shutdown_vulkan_gui(r_GraphicsDevice->get_state().logicalDevice, m_ImguiPool);
	}

	void VulkanGUI::begin_frame_impl()
	{
		begin_vulkan_frame();
	}
	void VulkanGUI::render_impl()
	{
		m_CommandBuffer->begin_render();

		render_vulkan_frame(m_CommandBuffer->get_command_buffer());

		m_CommandBuffer->end_render();
	}
	void VulkanGUI::end_frame_impl()
	{
		end_vulkan_frame();
	}

	void VulkanGUI::add_texture_impl(texture_handle handle)
	{
		Texture& texture = r_GraphicsDevice->get_native_texture(handle);
		descriptorSets[handle] = ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanGUI::remove_texture_impl(texture_handle handle)
	{
		ImGui_ImplVulkan_RemoveTexture(descriptorSets.at(handle));
	}
}
