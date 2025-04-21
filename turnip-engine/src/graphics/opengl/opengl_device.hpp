#pragma once
#include "core/free_list.hpp"
#include "graphics/graphics_device.hpp"

#include "opengl_common.hpp"
#include "opengl_buffer.hpp"
#include "opengl_texture.hpp"
#include "opengl_shader.hpp"
#include "opengl_pipeline.hpp"

namespace tur::gl
{
	class CommandBufferGL;

	class GraphicsDeviceGL : public BaseGraphicsDevice<GraphicsDeviceGL>
	{
		friend class BaseGraphicsDevice<GraphicsDeviceGL>;

	protected:
		void initialize_impl(NON_OWNING Window* window, const ConfigData&);
		void present_impl();

	protected:
		CommandBufferGL create_command_buffer_impl();

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
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);
		texture_handle create_texture_impl(const TextureDescriptor& descriptor);
		void update_texture_impl(texture_handle handle, const TextureAsset& asset);
		void destroy_texture_impl(texture_handle handle);

	protected:
		buffer_handle create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 size);
		void update_buffer_impl(buffer_handle handle, const DataBuffer& data, u32 offset = 0);
		void* map_buffer_impl(buffer_handle handle, u32 offset, u32 length, AccessFlags flags);
		void copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset = 0, u32 dstOffset = 0);
		void destroy_buffer_impl(buffer_handle handle);

	public:
		inline free_list<Buffer>&   get_buffers()   { return m_Buffers; }
		inline free_list<Texture>&  get_textures()  { return m_Textures; }
		inline free_list<Shader>&   get_shaders()   { return m_Shaders; }
		inline free_list<Pipeline>& get_pipelines() { return m_Pipelines; }

	private:
		free_list<Buffer>   m_Buffers;
		free_list<Texture>  m_Textures;
		free_list<Shader>   m_Shaders;
		free_list<Pipeline> m_Pipelines;

	private:
		NON_OWNING Window* r_Window = nullptr;
	};
}