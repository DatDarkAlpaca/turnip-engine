#pragma once
#include <imgui_impl_vulkan.h>

#include "common.hpp"
#include "graphics/gui_system.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_deletion_queue.hpp"

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
		void shutdown_impl();

	protected:
		void set_clear_color_impl(const Color& color);

	protected:
		void begin_frame_impl();
		void render_impl();
		void end_frame_impl();

	protected:
		void add_texture_impl(texture_handle handle);

		void remove_texture_impl(texture_handle handle);

		bool texture_button_impl(texture_handle handle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int framePadding, const ImVec4& bgColor, const ImVec4& tintColor);
		void texture_impl(texture_handle handle, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor);

	private:
		NON_OWNING GraphicsDeviceVulkan* r_GraphicsDevice = nullptr;
		tur_unique<CommandBufferVulkan> m_CommandBuffer;
		vk::DescriptorPool m_ImguiPool;

	private:
		std::unordered_map<texture_handle, VkDescriptorSet> descriptorSets;
		Color m_ClearColor;
	};
}