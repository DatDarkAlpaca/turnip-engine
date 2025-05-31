#pragma once
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

	private:
		NON_OWNING GraphicsDeviceVulkan* r_GraphicsDevice = nullptr;
		tur_unique<CommandBufferVulkan> m_CommandBuffer;
	};
}