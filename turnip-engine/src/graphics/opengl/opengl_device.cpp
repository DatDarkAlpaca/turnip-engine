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
	void GraphicsDeviceGL::shutdown_impl()
	{
		/* Blank */
	}

	void GraphicsDeviceGL::begin_impl()
	{
		/* Blank */
	}
	void GraphicsDeviceGL::present_impl()
	{
		present_opengl_window(r_Window);
	}
	void GraphicsDeviceGL::submit_impl()
	{
		/* Blank */
	}
	void GraphicsDeviceGL::end_impl()
	{
		/* Blank */
	}

	CommandBufferGL GraphicsDeviceGL::create_command_buffer_impl()
	{
		return CommandBufferGL(this);
	}
	OpenGLGUI GraphicsDeviceGL::create_gui_system_impl()
	{
		return OpenGLGUI(this);
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
	void GraphicsDeviceGL::destroy_shader_impl(shader_handle textureHandle)
	{
		glDeleteShader(m_Shaders.get(textureHandle).handle);
		m_Shaders.remove(textureHandle);
	}

	descriptor_handle GraphicsDeviceGL::create_descriptors_impl(const DescriptorSetLayoutDescriptor& descriptor)
	{
		return m_Descriptors.add({});
	}
	descriptor_set_handle GraphicsDeviceGL::create_descriptor_set_impl(descriptor_handle descriptorHandle)
	{
		descriptor_set_handle handle = m_DescriptorSets.add({});

		m_Descriptors.get(descriptorHandle).sets.push_back(handle);
		
		return handle;
	}
	void GraphicsDeviceGL::update_descriptor_resource_impl(descriptor_set_handle descriptorSetHandle, handle_type resourceHandle, DescriptorType type, u32 binding)
	{
		gl_handle glType = get_descriptor_set_type(type);

		DescriptorSet& descriptorSet = m_DescriptorSets.get(descriptorSetHandle);

		switch (type)
		{
			case DescriptorType::STORAGE_BUFFER:
			case DescriptorType::UNIFORM_BUFFER:
			{
				glBindBufferBase(glType, binding, m_Buffers.get(resourceHandle).handle);
				descriptorSet.boundBuffers.push_back(resourceHandle);
			} break;

			case DescriptorType::COMBINED_IMAGE_SAMPLER:
			{
				glBindTextureUnit(binding, m_Textures.get(resourceHandle).handle);
				descriptorSet.boundTextures.push_back(resourceHandle);
			} break;
		}
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

		update_texture_impl(textureIndex, asset);
		return textureIndex;
	}
	texture_handle GraphicsDeviceGL::create_texture_impl(const TextureDescriptor& descriptor)
	{
		gl_handle textureType = get_texture_type(descriptor.type);
		gl_handle textureFormat = get_texture_format(descriptor.format);

		gl_handle textureID;
		glCreateTextures(textureType, 1, &textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

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

		switch (descriptor.type)
		{
			case TextureType::TEXTURE_2D:
				glTextureStorage2D(
					textureID,
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
					textureID,
					1,
					textureFormat,
					descriptor.width,
					descriptor.height,
					descriptor.depth
				);
				break;
		}

		gl::Texture texture;
		{
			texture.handle = textureID;
			texture.descriptor = descriptor;
		}

		return static_cast<texture_handle>(m_Textures.add(texture));
	}
	void GraphicsDeviceGL::update_texture_impl(texture_handle textureHandle, const TextureAsset& asset)
	{
		gl::Texture& texture = m_Textures.get(textureHandle);
		
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
	void GraphicsDeviceGL::destroy_texture_impl(texture_handle textureHandle)
	{
		auto& texture = m_Textures.remove(textureHandle);
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
	void GraphicsDeviceGL::update_buffer_impl(buffer_handle textureHandle, const DataBuffer& data, u32 offset)
	{
		auto& buffer = m_Buffers.get(textureHandle);		
		glNamedBufferSubData(buffer.handle, offset, data.size, data.data);
	}
	void* GraphicsDeviceGL::map_buffer_impl(buffer_handle textureHandle, u32 offset, u32 length, AccessFlags flags)
	{
		return glMapNamedBufferRange(
			m_Buffers.get(textureHandle).handle,
			offset,
			length,
			get_buffer_access_flags(flags)
		);
	}
	void GraphicsDeviceGL::copy_buffer_impl(buffer_handle source, buffer_handle destination, u32 size, u32 srcOffset, u32 dstOffset)
	{
		auto& srcBuffer = m_Buffers.get(source);
		auto& dstBuffer = m_Buffers.get(destination);

		glCopyNamedBufferSubData(srcBuffer.handle, dstBuffer.handle, srcOffset, dstOffset, size);
	}
	void GraphicsDeviceGL::copy_buffer_to_texture_impl(buffer_handle source, texture_handle destination, u32 width, u32 height)
	{
		TUR_LOG_CRITICAL("UNIMPLEMENTED");
	}
	void GraphicsDeviceGL::destroy_buffer_impl(buffer_handle textureHandle)
	{
		auto& buffer = m_Buffers.remove(textureHandle);
		glDeleteBuffers(1, &buffer.handle);
	}

	render_target_handle GraphicsDeviceGL::create_render_target_impl(const RenderTargetDescriptor& descriptor)
	{
		u32 framebufferID;
		glCreateFramebuffers(1, &framebufferID);

		u32 index = 0;
		for (const auto& colorAttachment : descriptor.colorAttachments)
		{
			gl_handle colorAttachmentHandle = m_Textures.get(colorAttachment).handle;
			glNamedFramebufferTexture(framebufferID, GL_COLOR_ATTACHMENT0 + index, colorAttachmentHandle, 0);
			
			++index;
		}

		u32 rbo;
		glCreateRenderbuffers(1, &rbo);
		glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, descriptor.width, descriptor.height);
		glNamedFramebufferRenderbuffer(framebufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// use depth texture

		if (glCheckNamedFramebufferStatus(framebufferID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			TUR_LOG_CRITICAL("Incomplete framebuffer: ", framebufferID);

		return m_RenderTargets.add({ framebufferID, descriptor });
	}
	void GraphicsDeviceGL::resize_render_target_impl(render_target_handle textureHandle, u32 width, u32 height)
	{
		RenderTarget renderTarget = m_RenderTargets.get(textureHandle);
		renderTarget.descriptor.width  = width;
		renderTarget.descriptor.height = height;

		size_t index = 0;
		for (texture_handle& colorAttachment : renderTarget.descriptor.colorAttachments)
		{
			TextureDescriptor textureDescriptor = m_Textures.get(colorAttachment).descriptor;
			destroy_texture(colorAttachment);
			
			textureDescriptor.width = width;
			textureDescriptor.height = height;

			colorAttachment = m_Textures.get(create_texture(textureDescriptor)).handle;
			glNamedFramebufferTexture(renderTarget.handle, GL_COLOR_ATTACHMENT0 + static_cast<u32>(index), colorAttachment, 0);
		}

		u32 rbo;
		glCreateRenderbuffers(1, &rbo);
		glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, width, height);
		glNamedFramebufferRenderbuffer(renderTarget.handle, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckNamedFramebufferStatus(renderTarget.handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			TUR_LOG_CRITICAL("Incomplete framebuffer: ", renderTarget.handle);
	}
	void GraphicsDeviceGL::destroy_render_target_impl(render_target_handle textureHandle)
	{
		auto& framebuffer = m_RenderTargets.remove(textureHandle);
		glDeleteFramebuffers(1, &framebuffer.handle);
	}
}