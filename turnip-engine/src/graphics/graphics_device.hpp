#pragma once
#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"

#include "core/config/config_data.hpp"
#include "graphics_specification.hpp"
#include "core/assets/texture_asset.hpp"

namespace tur
{
	template<typename Backend>
	class BaseGraphicsDevice
	{
	public:
		void initialize(NON_OWNING Window* window, const ConfigData& configData)
		{
			static_cast<Backend*>(this)->initialize_impl(window, configData);
		}
		void present()
		{
			static_cast<Backend*>(this)->present_impl();
		}

	public:
		auto create_command_buffer()
		{
			return static_cast<Backend*>(this)->create_command_buffer_impl();
		}
		void initialize_gui_graphics_system()
		{
			static_cast<Backend*>(this)->initialize_gui_graphics_system_impl();
		}

	public:
		buffer_handle create_default_buffer(const BufferDescriptor& descriptor, const DataBuffer& data)
		{
			return static_cast<Backend*>(this)->create_default_buffer_impl(descriptor, data);
		}
		buffer_handle create_buffer(const BufferDescriptor& descriptor, u32 size)
		{
			return static_cast<Backend*>(this)->create_buffer_impl(descriptor, size);
		}
		void update_buffer(buffer_handle handle, const DataBuffer& data, u32 offset = 0)
		{
			static_cast<Backend*>(this)->update_buffer_impl(handle, data, offset);
		}
		void copy_buffer(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset = 0, u32 dstOffset = 0)
		{
			static_cast<Backend*>(this)->copy_buffer_impl(source, destination, size, srcOffset, dstOffset);
		}
		void destroy_buffer(buffer_handle handle)
		{
			static_cast<Backend*>(this)->destroy_buffer_impl(handle);
		};

	public:
		texture_handle create_texture(const TextureDescriptor& descriptor, const TextureAsset& asset)
		{
			return static_cast<Backend*>(this)->create_texture_impl(descriptor, asset);
		}
		void destroy_texture(texture_handle handle)
		{
			static_cast<Backend*>(this)->destroy_texture_impl(handle);
		}

	public:
		shader_handle create_shader(const ShaderDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_shader_impl(descriptor);
		}

		void destroy_shader(shader_handle handle)
		{
			static_cast<Backend*>(this)->destroy_shader_impl(handle);
		}

	public:
		pipeline_handle create_graphics_pipeline(const PipelineDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_graphics_pipeline_impl(descriptor);
		}

	protected:
		BaseGraphicsDevice() = default;
	};
}