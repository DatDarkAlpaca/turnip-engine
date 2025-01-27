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

	buffer_handle GraphicsDeviceGL::create_buffer_impl(const BufferDescriptor& descriptor, u32 bufferSize)
	{
		gl_handle bufferID;
		glGenBuffers(1, &bufferID);

		gl_handle bufferType = get_buffer_type(descriptor.type);

		glBindBuffer(bufferType, bufferID);
		glBufferData(
			bufferType,
			bufferSize,
			nullptr,
			get_buffer_usage(descriptor.usage)
		);

		glBindBuffer(bufferType, 0);

		gl::Buffer buffer;
		buffer.descriptor = descriptor;
		buffer.handle = bufferID;

		return static_cast<buffer_handle>(m_Buffers.add(buffer));
	}
	buffer_handle GraphicsDeviceGL::create_buffer_impl(const BufferDescriptor& descriptor, const DataBuffer& data)
	{
		gl_handle bufferID;
		glGenBuffers(1, &bufferID);

		gl_handle bufferType = get_buffer_type(descriptor.type);

		glBindBuffer(bufferType, bufferID);
		glBufferData(
			bufferType,
			data.size,
			data.data,
			get_buffer_usage(descriptor.usage)
		);

		glBindBuffer(bufferType, 0);

		gl::Buffer buffer;
		buffer.descriptor = descriptor;
		buffer.handle = bufferID;

		return static_cast<buffer_handle>(m_Buffers.add(buffer));
	}
	void GraphicsDeviceGL::update_buffer_impl(buffer_handle handle, const DataBuffer& data)
	{
		auto& buffer = m_Buffers.get(handle);
		glBindBuffer(get_buffer_type(buffer.descriptor.type), buffer.handle);

		glBufferSubData(get_buffer_type(buffer.descriptor.type), 0, data.size, data.data);

		glBindBuffer(get_buffer_type(buffer.descriptor.type), 0);
	}
	void* GraphicsDeviceGL::map_buffer_impl(buffer_handle handle)
	{
		auto& buffer = m_Buffers.get(handle);
		glBindBuffer(get_buffer_type(buffer.descriptor.type), buffer.handle);

		void* mapped = glMapBuffer(get_buffer_type(buffer.descriptor.type), GL_WRITE_ONLY);

		glBindBuffer(get_buffer_type(buffer.descriptor.type), 0);

		return mapped;
	}
	void GraphicsDeviceGL::unmap_buffer_impl(buffer_handle handle)
	{
		auto& buffer = m_Buffers.get(handle);
		glBindBuffer(get_buffer_type(buffer.descriptor.type), buffer.handle);

		glUnmapBuffer(get_buffer_type(buffer.descriptor.type));

		glBindBuffer(get_buffer_type(buffer.descriptor.type), 0);
	}
	void GraphicsDeviceGL::destroy_buffer_impl(buffer_handle handle)
	{
		auto& buffer = m_Buffers.get(handle);
		glDeleteBuffers(1, &buffer.handle);
	}

	shader_handle GraphicsDeviceGL::create_shader_impl(const ShaderDescriptor& descriptor)
	{
		std::string contents = read_file(descriptor.filepath);
		const char* cString = contents.c_str();

		u32 shaderID = glCreateShader(get_shader_type(descriptor.type));
		glShaderSource(shaderID, 1, &cString, NULL);
		glCompileShader(shaderID);
		check_compile_error(shaderID, descriptor.type);

		Shader shader;
		shader.handle = shaderID;

		return static_cast<texture_handle>(m_Shaders.add(shader));
	}
	void GraphicsDeviceGL::destroy_shader_impl(shader_handle handle)
	{
		glDeleteShader(m_Shaders.get(handle).handle);
		m_Shaders.remove(handle);
	}

	texture_handle GraphicsDeviceGL::create_texture_impl(const TextureDescriptor& descriptor, const TextureAsset& asset)
	{
		gl_handle textureID;
		glGenTextures(1, &textureID);

		gl_handle textureType = get_texture_type(descriptor.type);
		gl_handle textureFormat = get_texture_format(descriptor.format);
		bool generateMipmaps = descriptor.generateMipmaps;

		// TODO: implement float textures | texture formats.

		gl_handle dataFormat = get_texture_data_format(asset.dataFormat);
		gl_handle dataFormatType = asset.floatTexture ? GL_FLOAT : GL_UNSIGNED_BYTE;

		glBindTexture(textureType, textureID);
		switch (descriptor.type)
		{
			case TextureType::TEXTURE_2D:
			{
				glTexImage2D(
					textureType,
					0,
					textureFormat,
					descriptor.width,
					descriptor.height,
					0,
					dataFormat,
					dataFormatType,
					asset.data.data
				);
				
			} break;

			case TextureType::TEXTURE_3D:
			{
				// TODO: implement
			} break;

			case TextureType::CUBE_MAP:
			{
				// TODO: implement
			} break;
		}

		// Parameters:
		{
			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, get_wrap_mode(descriptor.wrapS));
			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, get_wrap_mode(descriptor.wrapT));
			if (descriptor.type == TextureType::TEXTURE_3D)
				glTexParameteri(textureType, GL_TEXTURE_WRAP_R, get_wrap_mode(descriptor.wrapR));

			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, get_filter_mode(descriptor.minFilter, generateMipmaps));
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, get_filter_mode(descriptor.magFilter, false));

			if (generateMipmaps)
				glGenerateMipmap(textureType);
		}
		
		gl::Texture texture;
		{
			texture.handle = textureID;
			texture.descriptor = descriptor;
		}
		
		glBindTexture(textureType, 0);

		return static_cast<texture_handle>(m_Textures.add(texture));
	}
	void GraphicsDeviceGL::destroy_texture_impl(texture_handle handle)
	{
		auto& texture = m_Textures.get(handle);
		glDeleteTextures(1, &texture.handle);
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
}