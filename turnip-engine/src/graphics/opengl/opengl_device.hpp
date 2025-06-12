#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

#include "opengl_common.hpp"
#include "opengl_buffer.hpp"
#include "opengl_texture.hpp"
#include "opengl_shader.hpp"
#include "opengl_pipeline.hpp"
#include "opengl_descriptor.hpp"
#include "opengl_gui_system.hpp"
#include "opengl_rendertarget.hpp"

namespace tur::gl
{
	class CommandBufferGL;

	class GraphicsDeviceGL : public BaseGraphicsDevice<GraphicsDeviceGL>
	{
		friend class BaseGraphicsDevice<GraphicsDeviceGL>;
		friend class OpenGLGUI;
		friend class CommandBufferGL;

	protected:
		void initialize_impl(NON_OWNING Window* window, const ConfigData&);
		void shutdown_impl();
		
		void begin_impl();
		void present_impl();
		void submit_impl();
		void end_impl();

	protected:
		CommandBufferGL create_command_buffer_impl();
		OpenGLGUI create_gui_system_impl();

	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);
		void destroy_shader_impl(shader_handle textureHandle);

	protected:
		descriptor_handle create_descriptors_impl(const DescriptorSetLayoutDescriptor& descriptor);
		descriptor_set_handle create_descriptor_set_impl(descriptor_handle descriptorHandle);
		void update_descriptor_resource_impl(descriptor_set_handle descriptorSetHandle, handle_type resourceHandle, DescriptorType type, u32 binding);

	protected:
		pipeline_handle create_graphics_pipeline_impl(const PipelineDescriptor& descriptor);

	protected:
		buffer_handle create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 size);
		void update_buffer_impl(buffer_handle textureHandle, const DataBuffer& data, u32 offset = 0);
		void* map_buffer_impl(buffer_handle textureHandle, u32 offset, u32 length, AccessFlags flags);
		void copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset = 0, u32 dstOffset = 0);
		void copy_buffer_to_texture_impl(buffer_handle source, texture_handle destination, u32 width, u32 height);
		void destroy_buffer_impl(buffer_handle textureHandle);

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		texture_handle create_texture_impl(const TextureDescriptor& descriptor);
		void update_texture_impl(texture_handle textureHandle, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle textureHandle);

	protected:
		render_target_handle create_render_target_impl(const RenderTargetDescriptor& descriptor);
		void resize_render_target_impl(render_target_handle textureHandle, u32 width, u32 height);
		void destroy_render_target_impl(render_target_handle textureHandle);

	private:
		inline free_list<Buffer>&			get_buffers()			{ return m_Buffers; }
		inline free_list<Descriptor>&		get_descriptors()		{ return m_Descriptors; }
		inline free_list<DescriptorSet>&	get_descriptor_sets()	{ return m_DescriptorSets; }
		inline free_list<Texture>&			get_textures()			{ return m_Textures; }
		inline free_list<Shader>&			get_shaders()			{ return m_Shaders; }
		inline free_list<Pipeline>&			get_pipelines()			{ return m_Pipelines; }
		inline free_list<RenderTarget>&		get_render_targets()	{ return m_RenderTargets; }

	private:
		free_list<Buffer>			m_Buffers;
		free_list<Descriptor>		m_Descriptors;
		free_list<DescriptorSet>	m_DescriptorSets;
		free_list<Texture>			m_Textures;
		free_list<Shader>			m_Shaders;
		free_list<Pipeline>			m_Pipelines;
		free_list<RenderTarget>		m_RenderTargets;

	private:
		NON_OWNING Window* r_Window = nullptr;
	};
}