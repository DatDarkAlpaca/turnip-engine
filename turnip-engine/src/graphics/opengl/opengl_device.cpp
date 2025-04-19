#include "pch.hpp"

#include "core/buffer.hpp"
#include "utils/file.hpp"

#include "opengl_device.hpp"
#include "opengl_command_buffer.hpp"

#include "platform/opengl_context.hpp"

namespace tur::gl
{
	void GraphicsDeviceGL::initialize_impl(NON_OWNING Window* window, const ConfigData&)
	{
		r_Window = window;
	}
	void GraphicsDeviceGL::present_impl()
	{
		present_opengl_window(r_Window);
	}

	CommandBufferGL GraphicsDeviceGL::create_command_buffer_impl()
	{
		return CommandBufferGL(this);
	}

	void GraphicsDeviceGL::initialize_gui_graphics_system_impl()
	{
		initialize_opengl_gui(r_Window);
	}
	void GraphicsDeviceGL::begin_gui_frame_impl()
	{
		begin_opengl_frame();
	}
	void GraphicsDeviceGL::end_gui_frame_impl()
	{
		end_opengl_frame();
	}

	shader_handle GraphicsDeviceGL::create_shader_impl(const ShaderDescriptor& descriptor)
	{
		std::string contents = read_file(descriptor.filepath);
		const char* cString = contents.c_str();

		u32 shaderID = glCreateShader(get_shader_type(descriptor.type));
		glShaderSource(shaderID, 1, &cString, NULL);
		glCompileShader(shaderID);
		check_compile_error(shaderID, descriptor.type);

		Shader shader = {};
		shader.handle = shaderID;

		return static_cast<texture_handle>(m_Shaders.add(shader));
	}
	void GraphicsDeviceGL::destroy_shader_impl(shader_handle handle)
	{
		glDeleteShader(m_Shaders.get(handle).handle);
		m_Shaders.remove(handle);
	}

	pipeline_handle GraphicsDeviceGL::create_graphics_pipeline_impl(const PipelineDescriptor& descriptor)
	{
		gl_handle pipelineID = glCreateProgram();

		const shader_handle& vertexShader = descriptor.vertexShader;
		const shader_handle& tesselationControlShader = descriptor.tesselationControlShader;
		const shader_handle& tesselationEvaluationShader = descriptor.tesselationEvaluationShader;
		const shader_handle& geometryShader = descriptor.geometryShader;
		const shader_handle& fragmentShader = descriptor.fragmentShader;

		{
			if (vertexShader == invalid_handle)
				TUR_LOG_CRITICAL("Vertex shader not specified in a graphics pipeline");

			glAttachShader(pipelineID, m_Shaders.get(vertexShader).handle);

			if (tesselationControlShader != invalid_handle)
				glAttachShader(pipelineID, m_Shaders.get(tesselationControlShader).handle);

			if (tesselationEvaluationShader != invalid_handle)
				glAttachShader(pipelineID, m_Shaders.get(tesselationEvaluationShader).handle);

			if (geometryShader != invalid_handle)
				glAttachShader(pipelineID, m_Shaders.get(geometryShader).handle);

			if (fragmentShader == invalid_handle)
				TUR_LOG_CRITICAL("Fragment shader not specified in a graphics pipeline");

			glAttachShader(pipelineID, m_Shaders.get(fragmentShader).handle);
		}

		glLinkProgram(pipelineID);
		glValidateProgram(pipelineID);

		check_program_link_errors(pipelineID);

		// Shader destruction:
		{
			destroy_shader(vertexShader);

			if (tesselationControlShader != invalid_handle)
				destroy_shader(tesselationControlShader);

			if (tesselationEvaluationShader != invalid_handle)
				destroy_shader(tesselationEvaluationShader);

			if (geometryShader != invalid_handle)
				destroy_shader(geometryShader);

			destroy_shader(fragmentShader);
		}

		Pipeline pipeline;
		pipeline.handle = pipelineID;
		pipeline.descriptor = descriptor;

		return static_cast<texture_handle>(m_Pipelines.add(pipeline));
	}
	
	texture_handle GraphicsDeviceGL::create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
	{
		texture_handle textureIndex = create_texture(descriptor);
		auto& texture = m_Textures.get(textureIndex);

		gl_handle textureFormat = get_texture_format(descriptor.format);

		switch (descriptor.type)
		{
			case TextureType::TEXTURE_2D:
				glTextureStorage2D(
					texture.handle,
					1,
					textureFormat,
					descriptor.width,
					descriptor.height
				);
				break;

			case TextureType::CUBE_MAP:
			case TextureType::ARRAY_TEXTURE_2D:
			case TextureType::TEXTURE_3D:
				glTextureStorage3D(
					texture.handle,
					1,
					textureFormat,
					descriptor.width,
					descriptor.height,
					descriptor.depth
				);
				break;
		}

		update_texture_impl(textureIndex, asset);
		return textureIndex;
	}
	texture_handle GraphicsDeviceGL::create_texture_impl(const TextureDescriptor& descriptor)
	{
		gl_handle textureType = get_texture_type(descriptor.type);

		gl_handle textureID;
		glCreateTextures(textureType, 1, &textureID);

		{
			bool generateMipmaps = descriptor.generateMipmaps;

			glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, get_wrap_mode(descriptor.wrapS));
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, get_wrap_mode(descriptor.wrapT));
			if (descriptor.type == TextureType::TEXTURE_3D)
				glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, get_wrap_mode(descriptor.wrapR));

			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, get_filter_mode(descriptor.minFilter, generateMipmaps));
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, get_filter_mode(descriptor.magFilter, false));

			if (generateMipmaps)
				glGenerateTextureMipmap(textureID);
		}

		gl::Texture texture;
		{
			texture.handle = textureID;
			texture.descriptor = descriptor;
		}

		return static_cast<texture_handle>(m_Textures.add(texture));
	}
	void GraphicsDeviceGL::update_texture_impl(texture_handle handle, const TextureAsset& asset)
	{
		auto& texture = m_Textures.get(handle);
		
		gl_handle dataFormat = get_texture_data_format(asset.dataFormat);
		gl_handle dataType = !asset.floatTexture ? GL_UNSIGNED_BYTE : GL_FLOAT;

		switch (texture.descriptor.type)
		{
			case TextureType::TEXTURE_2D:
				glTextureSubImage2D(texture.handle, 0, asset.xOffset, asset.yOffset, asset.width, asset.height,
					dataFormat, dataType, asset.data.data);
				break;

			case TextureType::CUBE_MAP:
			case TextureType::ARRAY_TEXTURE_2D:
			case TextureType::TEXTURE_3D:
				glTextureSubImage3D(texture.handle, 0, asset.xOffset, asset.yOffset, asset.zOffset,
					asset.width, asset.height, asset.depth, dataFormat, dataType, asset.data.data);
				break;

			default:
				TUR_LOG_ERROR("Invalid texture descriptor type on update");
		}
	}
	void GraphicsDeviceGL::destroy_texture_impl(texture_handle handle)
	{
		auto& texture = m_Textures.get(handle);
		glDeleteTextures(1, &texture.handle);
	}

	buffer_handle GraphicsDeviceGL::create_default_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data)
	{
		gl_handle bufferID;
		glCreateBuffers(1, &bufferID);
		glNamedBufferStorage(bufferID, data.size, data.data, get_buffer_usage(descriptor.usage));

		gl::Buffer buffer;
		buffer.descriptor = descriptor;
		buffer.handle = bufferID;

		return static_cast<buffer_handle>(m_Buffers.add(buffer));
	}
	buffer_handle GraphicsDeviceGL::create_buffer_impl(const BufferDescriptor& descriptor, u32 size)
	{
		return create_default_buffer_impl(descriptor, { nullptr, size });
	}
	void GraphicsDeviceGL::update_buffer_impl(buffer_handle handle, const DataBuffer& data, u32 offset)
	{
		auto& buffer = m_Buffers.get(handle);		
		glNamedBufferSubData(buffer.handle, offset, data.size, data.data);
	}
	void GraphicsDeviceGL::copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset)
	{
		auto& srcBuffer = m_Buffers.get(source);
		auto& dstBuffer = m_Buffers.get(destination);

		glCopyNamedBufferSubData(srcBuffer.handle, dstBuffer.handle, srcOffset, dstOffset, size);
	}
	void GraphicsDeviceGL::destroy_buffer_impl(buffer_handle handle)
	{
		auto& buffer = m_Buffers.get(handle);
		glDeleteBuffers(1, &buffer.handle);
	}

	void GraphicsDeviceGL::update_descriptor_set_impl(buffer_handle handle)
	{
		/* Blank */
	}
}