#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

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
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 bufferSize)
		{
			return invalid_handle;
		}
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data)
		{
			return invalid_handle;
		}
		void update_buffer_impl(buffer_handle handle, const DataBuffer& data)
		{

		}
		void* map_buffer_impl(buffer_handle handle)
		{

		}
		void unmap_buffer_impl(buffer_handle handle)
		{

		}
		void destroy_buffer_impl(buffer_handle handle)
		{

		}

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle handle)
		{

		}

	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);
		void destroy_shader_impl(shader_handle handle);

	protected:
		pipeline_handle create_pipeline_impl(const PipelineDescriptor& descriptor);

	public:
		inline free_list<vk::ShaderModule>& get_shader_modules() { return m_ShaderModules; }
		inline VulkanState& get_state() { return m_State; }

	private:
		free_list<vk::ShaderModule> m_ShaderModules;
		free_list<vk::Pipeline> m_Pipelines;
		free_list<Texture> m_Textures;
		VulkanState m_State;
	};
}