#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/vulkan_state.hpp"

namespace tur::vulkan
{
	class CommandBufferVulkan;

	class GraphicsDeviceVulkan : public BaseGraphicsDevice<GraphicsDeviceVulkan>
	{
		friend class BaseGraphicsDevice<GraphicsDeviceVulkan>;

	protected:
		void initialize_impl(NON_OWNING Window* window, const ConfigData& configData);
		void present_impl();

	protected:
		CommandBufferVulkan create_command_buffer_impl();

	protected:
		buffer_handle create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 size);
		void update_buffer_impl(buffer_handle handle, const DataBuffer& data, u32 offset);
		void copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset);
		void destroy_buffer_impl(buffer_handle handle);

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle handle)
		{

		}

	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);
		void destroy_shader_impl(shader_handle handle);

	protected:
		pipeline_handle create_graphics_pipeline_impl(const PipelineDescriptor& descriptor);

	public:
		void recreate_swapchain();

	private:
		void submit_immediate_command(std::function<void()>&& function);

	public:
		inline free_list<vk::ShaderModule>& get_shader_modules() { return m_ShaderModules; }
		inline free_list<std::pair<vk::Pipeline, PipelineType>>& get_pipelines() { return m_Pipelines; }
		inline free_list<Texture>& get_textures() { return m_Textures; }
		inline free_list<Buffer>& get_buffers() { return m_Buffers; }
		inline VulkanState& get_state() { return m_State; }

	private:
		Window* r_Window = nullptr;
		VulkanState m_State;
		ConfigData m_ConfigData;

	private:
		free_list<vk::ShaderModule> m_ShaderModules;
		free_list<std::pair<vk::Pipeline, PipelineType>> m_Pipelines;
		free_list<Texture> m_Textures;
		free_list<Buffer> m_Buffers;

	private:
		vk::Fence m_ImmFence;
		vk::CommandPool m_ImmCommandPool;
		vk::CommandBuffer m_ImmCommandBuffer;
	};
}