#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

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
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
		{
			return invalid_handle;
		}

		void destroy_texture_impl(texture_handle handle)
		{

		}

	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor)
		{
			return invalid_handle;
		}

		void destroy_shader_impl(shader_handle handle)
		{

		}

	protected:
		pipeline_handle create_pipeline_impl(const PipelineDescriptor& descriptor)
		{
			return invalid_handle;
		}

	public:
		VulkanState& get_state() { return m_State; }

	private:
		VulkanState m_State;
	};
}