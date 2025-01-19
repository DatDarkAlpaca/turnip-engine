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
		void initialize_impl(NON_OWNING Window* window);

		void present_impl();

	public:
		CommandBufferGL create_command_buffer_impl();

	protected:
		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, u32 bufferSize);

		buffer_handle create_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data);

		void update_buffer_impl(buffer_handle handle, const DataBuffer& data);

		void* map_buffer_impl(buffer_handle handle);

		void unmap_buffer_impl(buffer_handle handle);

		void destroy_buffer_impl(buffer_handle handle);

	protected:
		shader_handle create_shader_impl(const ShaderDescriptor& descriptor);

		void destroy_shader_impl(shader_handle handle);

	protected:
		texture_handle create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset);

		void destroy_texture_impl(texture_handle handle);

	protected:
		pipeline_handle create_pipeline_impl(const PipelineDescriptor& descriptor);

	public:
		free_list<Buffer>&   get_buffers()   { return m_Buffers; }
		free_list<Texture>&  get_textures()  { return m_Textures; }
		free_list<Shader>&   get_shaders()   { return m_Shaders; }
		free_list<Pipeline>& get_pipelines() { return m_Pipelines; }

	private:
		tur_unique<Swapbuffer> m_Swapbuffer;

		free_list<Buffer>   m_Buffers;
		free_list<Texture>  m_Textures;
		free_list<Shader>   m_Shaders;
		free_list<Pipeline> m_Pipelines;

	private:
		NON_OWNING Window* r_Window = nullptr;
	};
}