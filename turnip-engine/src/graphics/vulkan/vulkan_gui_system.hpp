#pragma once
#include <imgui_impl_vulkan.h>

#include "common.hpp"
#include "graphics/gui_system.hpp"
#include "vulkan_command_buffer.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	class VulkanGUI : public BaseGUISystem<VulkanGUI>
	{
		friend class BaseGUISystem<VulkanGUI>;

	public:
		VulkanGUI(GraphicsDeviceVulkan* device);

	protected:
		void initialize_impl();

	protected:
		void begin_frame_impl();
		void render_impl();
		void end_frame_impl();

	protected:
		void add_texture_impl(texture_handle handle)
		{
			Texture& texture = r_GraphicsDevice->get_native_texture(handle);
			descriptorSets[handle] = ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		void remove_texture_impl(texture_handle handle)
		{
			ImGui_ImplVulkan_RemoveTexture(descriptorSets.at(handle));
		}

		bool texture_button_impl(texture_handle handle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int framePadding, const ImVec4& bgColor, const ImVec4& tintColor)
		{
			Texture& texture = r_GraphicsDevice->get_native_texture(handle);
			return ImGui::ImageButton("Image", (ImTextureID)descriptorSets[handle], size, uv0, uv1, bgColor, tintColor);
		}

		void texture_impl(texture_handle handle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor)
		{
			Texture& texture = r_GraphicsDevice->get_native_texture(handle);
			ImGui::Image((ImTextureID)descriptorSets[handle], size, uv0, uv1, tintColor, borderColor);
		}

	private:
		NON_OWNING GraphicsDeviceVulkan* r_GraphicsDevice = nullptr;
		tur_unique<CommandBufferVulkan> m_CommandBuffer;

	private:
		std::unordered_map<texture_handle, VkDescriptorSet> descriptorSets;
	};
}