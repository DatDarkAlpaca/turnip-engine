#include "pch.hpp"
#include "Utils/File.hpp"
#include "TypeConverters.hpp"
#include "GraphicsLayerGL.hpp"

namespace tur::gl
{
	tur_unique<ISwapchain> GraphicsLayerGL::CreateSwapChain(const SwapchainDescriptor& descriptor)
	{
		return tur::MakeUnique<SwapchainGL>(descriptor, r_Window);
	}
	tur_unique<ICommandBuffer> GraphicsLayerGL::CreateCommandBuffer()
	{
		return tur::MakeUnique<CommandBufferGL>(this);
	}

	shader_handle GraphicsLayerGL::CreateShader(const ShaderDescriptor& descriptor)
	{
		std::string contents = ReadFile(descriptor.filepath);
		const char* cString = contents.c_str();

		uint32_t shaderID = glCreateShader(GetShaderType(descriptor.type));
		glShaderSource(shaderID, 1, &cString, NULL);
		glCompileShader(shaderID);
		CheckCompileErrors(shaderID, descriptor.type);

		return m_Shaders.add(shaderID);
	}
	void GraphicsLayerGL::DestroyShader(shader_handle handle)
	{
		glDeleteShader(m_Shaders.get(handle));
		m_Shaders.remove(handle);
	}

	pipeline_handle GraphicsLayerGL::CreatePipeline(const PipelineDescriptor& descriptor)
	{
		gl_handle pipelineProgramID = glCreateProgram();

		shader_handle vertexShader = descriptor.vertexShader;
		shader_handle tesselationControlShader = descriptor.tesselationControlShader;
		shader_handle tesselationEvaluationShader = descriptor.tesselationEvaluationShader;
		shader_handle geometryShader = descriptor.geometryShader;
		shader_handle fragmentShader = descriptor.fragmentShader;

		// Shaders:
		{
			if (vertexShader == invalid_handle)
				TUR_LOG_CRITICAL("Vertex shader not specified in a graphics pipeline");
			glAttachShader(pipelineProgramID, m_Shaders.get(vertexShader));

			if (tesselationControlShader != invalid_handle)
				glAttachShader(pipelineProgramID, m_Shaders.get(tesselationControlShader));
					
			if (tesselationEvaluationShader != invalid_handle)
				glAttachShader(pipelineProgramID, m_Shaders.get(tesselationEvaluationShader));

			if (geometryShader != invalid_handle)
				glAttachShader(pipelineProgramID, m_Shaders.get(geometryShader));

			if (fragmentShader == invalid_handle)
				TUR_LOG_CRITICAL("Fragment shader not specified in a graphics pipeline");
			glAttachShader(pipelineProgramID, m_Shaders.get(fragmentShader));
		}

		glLinkProgram(pipelineProgramID);
		glValidateProgram(pipelineProgramID);

		CheckProgramLinkErrors(pipelineProgramID);

		// Shader destruction:
		{
			DestroyShader(vertexShader);
			
			if (tesselationControlShader != invalid_handle)
				DestroyShader(tesselationControlShader);

			if (tesselationEvaluationShader != invalid_handle)
				DestroyShader(tesselationEvaluationShader);

			if (geometryShader != invalid_handle)
				DestroyShader(geometryShader);

			DestroyShader(fragmentShader);
		}

		m_PipelineStates.add(descriptor);
		return m_Pipelines.add(pipelineProgramID);
	}
	void GraphicsLayerGL::DestroyPipeline(pipeline_handle handle)
	{
		glDeleteProgram(m_Pipelines.get(handle));

		m_Pipelines.remove(handle);
		m_PipelineStates.remove(handle);
	}

	buffer_handle GraphicsLayerGL::CreateBuffer(const BufferDescriptor& descriptor, const void* data)
	{
		uint32_t buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GetBufferUsage(descriptor.usage), buffer);
		glBufferData(GetBufferUsage(descriptor.usage), descriptor.size, data, GetBufferMemoryUsage(descriptor.memoryUsage));
		
		glBindBuffer(GetBufferUsage(descriptor.usage), 0);

		return m_Buffers.add(buffer);
	}
	void GraphicsLayerGL::DestroyBuffer(buffer_handle handle)
	{
		auto bufferID = m_Buffers.get(handle);
		glDeleteBuffers(1, &bufferID);
		m_Buffers.remove(handle);
	}

	texture_handle GraphicsLayerGL::CreateTexture(const TextureDescriptor& descriptor, const void* data)
	{
		// TODO: add texture types etc.

		gl_handle textureID;
		glGenTextures(1, &textureID);

		gl_handle target = GetTextureType(descriptor.type);
		gl_handle internalFormat = GetTextureFormat(descriptor.format);
		bool generateMipmaps = descriptor.mipLevels > 1 || descriptor.generateMipmaps;

		glBindTexture(target, textureID);
		
		switch (descriptor.type)
		{
			case TextureType::TEXTURE_2D:
			{
				glTexImage2D(
					target,
					0,
					internalFormat,
					descriptor.width,
					descriptor.height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					data
				);
			} break;
			

			case TextureType::TEXTURE_3D:
				TUR_LOG_CRITICAL("3D Textures not implemented in OpenGL");

			case TextureType::CUBE_MAP:
				TUR_LOG_CRITICAL("Cubemaps not implemented in OpenGL");
		}

		glTexParameteri(target, GL_TEXTURE_WRAP_S, GetWrapMode(descriptor.wrapS));
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GetWrapMode(descriptor.wrapT));
		if (descriptor.type == TextureType::TEXTURE_3D)
			glTexParameteri(target, GL_TEXTURE_WRAP_R, GetWrapMode(descriptor.wrapR));

		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GetFilterMode(descriptor.minFilter, generateMipmaps));
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GetFilterMode(descriptor.magFilter, false));

		if (generateMipmaps)
			glGenerateMipmap(target);

		glBindTexture(target, 0);

		return m_Textures.add(textureID);
	}
	void GraphicsLayerGL::DestroyTexture(texture_handle handle)
	{
		auto textureID = m_Textures.get(handle);
		glDeleteTextures(1, &textureID);
		m_Textures.remove(handle);
	}

	framebuffer_handle GraphicsLayerGL::CreateFramebuffer(const FramebufferDescriptor& descriptor)
	{
		gl_handle framebufferID;
		glGenFramebuffers(1, &framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return m_Framebuffers.add(framebufferID);
	}
	void GraphicsLayerGL::DestroyFramebuffer(framebuffer_handle handle)
	{
		auto framebufferID = m_Framebuffers.get(handle);
		glDeleteFramebuffers(1, &framebufferID);
		m_Framebuffers.remove(handle);
	}

	renderpass_handle GraphicsLayerGL::CreateRenderpass(const RenderpassDescriptor& descriptor)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, descriptor.framebuffer_handle);

		std::vector<gl_handle> drawBuffers;
		for (size_t i = 0; i < descriptor.attachments.size(); ++i)
		{
			const auto& attachment = descriptor.attachments[i];

			for (auto attachmentType : descriptor.mapping.at(RenderpassDescriptor::attachment_id(i)))
			{
				switch (attachmentType)
				{
					case AttachmentType::COLOR:
					{
						gl_handle texture;
						glGenTextures(1, &texture);
						glBindTexture(GL_TEXTURE_2D, texture);

						auto textureFormat = GetTextureFormat(attachment.imageFormat);
						glTexImage2D(
							GL_TEXTURE_2D, 
							0, 
							textureFormat, 
							descriptor.extent.width, descriptor.extent.height, 
							0, 
							GL_RGBA, 
							GL_UNSIGNED_BYTE, 
							nullptr
						);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(WrapMode::REPEAT));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(WrapMode::REPEAT));

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetFilterMode(FilterMode::LINEAR, false));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetFilterMode(FilterMode::LINEAR, false));

						gl_handle attachmentTypeHandle = GetFramebufferAttachmentType(attachmentType);
						glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTypeHandle, GL_TEXTURE_2D, texture, 0);

						glBindTexture(GL_TEXTURE_2D, 0);

						if (attachmentType == AttachmentType::COLOR)
							drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i));

					} break;

					case AttachmentType::DEPTH:
					case AttachmentType::STENCIL:
					case AttachmentType::DEPTH_STENCIL:
					{
						gl_handle rbo;

						glGenRenderbuffers(1, &rbo);
						glBindRenderbuffer(GL_RENDERBUFFER, rbo);

						gl_handle format = GetTextureFormat(attachment.imageFormat);
						glRenderbufferStorage(
							GL_RENDERBUFFER,
							format, 
							descriptor.extent.width, 
							descriptor.extent.height
						);

						gl_handle attachmentTypeHandle = GetFramebufferAttachmentType(attachmentType);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentTypeHandle, GL_RENDERBUFFER, rbo);
					} break;
				}
			}
		}

		if (!drawBuffers.empty())
			glDrawBuffers(static_cast<uint32_t>(drawBuffers.size()), drawBuffers.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			TUR_LOG_ERROR("Failed to create framebuffer");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return m_Renderpasses.add(descriptor.framebuffer_handle);
	}
	void GraphicsLayerGL::DestroyRenderpass(renderpass_handle handle)
	{
		auto framebufferID = m_Renderpasses.get(handle);
		DestroyFramebuffer(framebufferID);
		m_Renderpasses.remove(handle);
	}
}