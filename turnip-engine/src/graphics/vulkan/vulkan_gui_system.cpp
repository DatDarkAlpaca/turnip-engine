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

	void VulkanGUI::set_clear_color_impl(const Color& color)
	{
		m_ClearColor = color;
	}

	void VulkanGUI::begin_frame_impl()
	{
		begin_vulkan_frame();
	}
	void VulkanGUI::render_impl()
	{
		m_CommandBuffer->begin_render();
		m_CommandBuffer->clear(ClearFlags::COLOR, { m_ClearColor });

		render_vulkan_frame(m_CommandBuffer->get_command_buffer());

		m_CommandBuffer->end_render();
	}
	void VulkanGUI::end_frame_impl()
	{
		end_vulkan_frame();
	}

	void VulkanGUI::add_texture_impl(texture_handle textureHandle)
	{
		if (descriptorSets.find(textureHandle) != descriptorSets.end())
			return;

		Texture& texture = r_GraphicsDevice->get_textures().get(textureHandle);
		descriptorSets[textureHandle] = ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	void VulkanGUI::remove_texture_impl(texture_handle textureHandle)
	{
		ImGui_ImplVulkan_RemoveTexture(descriptorSets.at(textureHandle));

		descriptorSets.erase(textureHandle);
	}

	bool VulkanGUI::texture_button_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int framePadding, const ImVec4& bgColor, const ImVec4& tintColor)
	{
		Texture& texture = r_GraphicsDevice->get_textures().get(textureHandle);

		if(descriptorSets.find(textureHandle) == descriptorSets.end())
			return false;

		return ImGui::ImageButton("Image", (ImTextureID)descriptorSets[textureHandle], size, uv0, uv1, bgColor, tintColor);
	}
	void VulkanGUI::texture_impl(texture_handle textureHandle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor)
	{
		Texture& texture = r_GraphicsDevice->get_textures().get(textureHandle);
	
		if (descriptorSets.find(textureHandle) == descriptorSets.end())
			return;
		
		ImGui::Image((ImTextureID)descriptorSets[textureHandle], size, uv0, uv1, tintColor, borderColor);
	}
}
