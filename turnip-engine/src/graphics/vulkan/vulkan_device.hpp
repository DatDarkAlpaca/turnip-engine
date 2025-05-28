#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"
#include "objects/vulkan_state.hpp"

namespace tur::vulkan
{
	class CommandBufferVulkan;

	class GraphicsDeviceVulkan : public BaseGraphicsDevice<GraphicsDeviceVulkan>
	{
		friend class BaseGraphicsDevice<GraphicsDeviceVulkan>;
		friend class CommandBufferVulkan;

		friend void initialize_vulkan_gui(vulkan::GraphicsDeviceVulkan*);
		friend Pipeline create_graphics_pipeline(GraphicsDeviceVulkan&, const PipelineDescriptor&);

	public:
		void recreate_swapchain();

	protected:
		void initialize_impl(NON_OWNING Window* window, const ConfigData& configData);
		void present_impl();

	protected:
		CommandBufferVulkan create_command_buffer_impl();

	protected:
		void initialize_gui_graphics_system_impl();
		void begin_gui_frame_impl();
		void end_gui_frame_impl();
	
	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);
		void destroy_shader_impl(shader_handle handle);

	protected:
		pipeline_handle create_graphics_pipeline_impl(const PipelineDescriptor& descriptor);

	protected:
		buffer_handle create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 size);
		void update_buffer_impl(buffer_handle handle, const DataBuffer& data, u32 offset);
		void* map_buffer_impl(buffer_handle handle, u32 offset, u32 length, AccessFlags flags);
		void copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset);
		void destroy_buffer_impl(buffer_handle handle);

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		texture_handle create_texture_impl(const TextureDescriptor& descriptor);
		void update_texture_impl(texture_handle handle, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle handle);

	protected:
		render_target_handle create_render_target_impl(const RenderTargetDescriptor& descriptor);
		void resize_render_target_impl(render_target_handle handle, u32 width, u32 height);

	public:
		void submit_immediate_command(std::function<void()>&& function);

	private:
		inline free_list<vk::ShaderModule>& get_shader_modules() { return m_ShaderModules; }
		inline free_list<Pipeline>& get_pipelines() { return m_Pipelines; }
		inline free_list<Texture>& get_textures() { return m_Textures; }
		inline free_list<Buffer>& get_buffers() { return m_Buffers; }
		inline free_list<Texture>& get_render_targets() { return m_RenderTargets; }

		inline VulkanState& get_state() { return m_State; }
		inline vk::CommandBuffer get_imm_command_buffer() const { return m_ImmCommandBuffer; }
		inline Window* get_window() { return r_Window; }

	private:
		Window* r_Window = nullptr;
		VulkanState m_State;
		ConfigData m_ConfigData;

	private:
		free_list<vk::ShaderModule> m_ShaderModules;
		free_list<Pipeline> m_Pipelines;
		free_list<Texture> m_Textures;
		free_list<Buffer> m_Buffers;
		free_list<Texture> m_RenderTargets;

	private:
		vk::Fence m_ImmFence;
		vk::CommandPool m_ImmCommandPool;
		vk::CommandBuffer m_ImmCommandBuffer;
	};
}