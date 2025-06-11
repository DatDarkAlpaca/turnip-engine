#pragma once
#include <variant>

#include "objects/buffer.hpp"
#include "objects/texture.hpp"
#include "objects/pipeline.hpp"
#include "objects/render_target.hpp"
#include "objects/descriptor.hpp"

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
		void shutdown()
		{
			static_cast<Backend*>(this)->shutdown_impl();
		}

		void begin()
		{
			static_cast<Backend*>(this)->begin_impl();
		}
		void present()
		{
			static_cast<Backend*>(this)->present_impl();
		}
		void submit()
		{
			static_cast<Backend*>(this)->submit_impl();
		}
		void end()
		{
			static_cast<Backend*>(this)->end_impl();
		}

	public:
		auto create_command_buffer()
		{
			return static_cast<Backend*>(this)->create_command_buffer_impl();
		}

		auto create_gui_system()
		{
			return static_cast<Backend*>(this)->create_gui_system_impl();
		}

	public:
		shader_handle create_shader(const ShaderDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_shader_impl(descriptor);
		}
		void destroy_shader(shader_handle textureHandle)
		{
			static_cast<Backend*>(this)->destroy_shader_impl(textureHandle);
		}

	public:
		descriptor_handle create_descriptors(const DescriptorSetLayoutDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_descriptors_impl(descriptor);
		}
		descriptor_set_handle build_descriptor_set(descriptor_handle textureHandle)
		{
			return static_cast<Backend*>(this)->create_descriptor_set_impl(textureHandle);
		}
		void update_descriptor_resource(descriptor_set_handle descriptorSetHandle, handle_type resourceHandle, DescriptorType type, u32 binding)
		{
			static_cast<Backend*>(this)->update_descriptor_resource_impl(descriptorSetHandle, resourceHandle, type, binding);
		}

	public:
		pipeline_handle build_graphics_pipeline(const PipelineDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_graphics_pipeline_impl(descriptor);
		}

	public:
		buffer_handle create_default_buffer(const BufferDescriptor& descriptor, const DataBuffer& data)
		{
			return static_cast<Backend*>(this)->create_default_buffer_impl(descriptor, data);
		}
		buffer_handle build_buffer(const BufferDescriptor& descriptor, u32 size)
		{
			return static_cast<Backend*>(this)->create_buffer_impl(descriptor, size);
		}
		void update_buffer(buffer_handle textureHandle, const DataBuffer& data, u32 offset = 0)
		{
			static_cast<Backend*>(this)->update_buffer_impl(textureHandle, data, offset);
		}
		void* map_buffer(buffer_handle textureHandle, u32 offset, u32 length, AccessFlags flags)
		{
			return static_cast<Backend*>(this)->map_buffer_impl(textureHandle, offset, length, flags);
		}
		void copy_buffer(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset = 0, u32 dstOffset = 0)
		{
			static_cast<Backend*>(this)->copy_buffer_impl(source, destination, size, srcOffset, dstOffset);
		}
		void copy_buffer_to_texture(buffer_handle source, texture_handle destination, u32 width, u32 height)
		{
			static_cast<Backend*>(this)->copy_buffer_to_texture_impl(source, destination, width, height);
		}
		void destroy_buffer(buffer_handle& textureHandle)
		{
			static_cast<Backend*>(this)->destroy_buffer_impl(textureHandle);
		};

	public:
		texture_handle build_texture(const TextureDescriptor& descriptor, const TextureAsset& asset)
		{
			return static_cast<Backend*>(this)->create_texture_impl(descriptor, asset);
		}
		texture_handle build_texture(const TextureDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_texture_impl(descriptor);
		}
		void update_texture(texture_handle textureHandle, const TextureAsset& asset)
		{
			return static_cast<Backend*>(this)->update_texture_impl(textureHandle, asset);
		}
		void destroy_texture(texture_handle& textureHandle)
		{
			static_cast<Backend*>(this)->destroy_texture_impl(textureHandle);
		}

	public:
		render_target_handle create_render_target(const RenderTargetDescriptor& descriptor)
		{
			return static_cast<Backend*>(this)->create_render_target_impl(descriptor);
		}
		void resize_render_target(render_target_handle& textureHandle, u32 width, u32 height)
		{
			static_cast<Backend*>(this)->resize_render_target_impl(textureHandle, width, height);
		}
		void destroy_render_target(render_target_handle& textureHandle)
		{
			static_cast<Backend*>(this)->destroy_render_target_impl(textureHandle);
		}

	protected:
		BaseGraphicsDevice() = default;
	};
}