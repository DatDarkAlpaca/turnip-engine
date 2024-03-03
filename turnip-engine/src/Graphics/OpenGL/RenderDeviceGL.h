#pragma once
#include <glad/glad.h>

#include "Rendering/RenderDevice.h"
#include "Platform/Platform.h"
#include "Util/File.h"

#include "GraphicsCommandsGL.h"
#include "Diagnostics.h"
#include "Converters.h"
#include "Renderpass.h"
#include "Pipeline.h"
#include "Buffer.h"
#include "Shader.h"

namespace tur::gl
{
	class RenderDeviceGL : public RenderDevice
	{
	public:
		RenderDeviceGL(NON_OWNING Window* window)
			: r_Window(window)
		{
			CreateSwapchainRenderpass();
		}

	public:
		inline GraphicsAPI API() override { return GraphicsAPI::VULKAN; }

	public:
		RenderpassHandle CreateRenderpass(const RenderpassDescriptor& renderpassDescription)
		{
			/* TODO: Implementation required, but not necessary */
			return RenderpassHandle::INVALID;
		}

		BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) override
		{
			uint32_t target = gl::GetBufferUsageFlags(bufferDescription.usageFlags);
			uint32_t usage = gl::GetBuferDataStorageType(bufferDescription.dataStorage);

			gl::Buffer buffer;
			buffer.usageFlags = bufferDescription.usageFlags;

			glGenBuffers(1, &buffer.id);
			glBindBuffer(target, buffer.id);
			glBufferData(target, bufferDescription.dataSize, bufferDescription.data, usage);
			glBindBuffer(target, 0);

			m_Buffers.push_back(buffer);
			return BufferHandle(m_Buffers.size() - 1);
		}

		ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) override
		{
			auto shaderContents = ReadFile(shaderDescriptor.filepath);
			const char* cShaderCode = shaderContents.c_str();

			gl::Shader shader = {};
			shader.id = glCreateShader(gl::GetShaderTypeValue(shaderDescriptor.type));

			glShaderSource(shader.id, 1, &cShaderCode, NULL);
			glCompileShader(shader.id);

			gl::CheckCompileErrors(shader.id, shaderDescriptor.type);

			m_Shaders.push_back(shader);
			return ShaderHandle(m_Shaders.size() - 1);
		}

		PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) override
		{
			std::vector<gl::Shader> shaders;
			shaders.reserve(5);
			{
				shaders.push_back(GetShader(pipelineDescriptor.vertexShader));
				shaders.push_back(GetShader(pipelineDescriptor.fragmentShader));

				if(pipelineDescriptor.tesselationControlShader != ShaderHandle::INVALID)
					shaders.push_back(GetShader(pipelineDescriptor.tesselationControlShader));

				if (pipelineDescriptor.tesselationEvaluationShader != ShaderHandle::INVALID)
					shaders.push_back(GetShader(pipelineDescriptor.tesselationEvaluationShader));
				
				if (pipelineDescriptor.geometryShader != ShaderHandle::INVALID)
					shaders.push_back(GetShader(pipelineDescriptor.geometryShader));
			}

			gl::Pipeline pipeline;
			pipeline.state = pipelineDescriptor;

			gl::SetupPipelineShaders(pipeline, shaders);

			m_Pipelines.push_back(pipeline);
			return PipelineStateHandle(m_Pipelines.size() - 1);
		}

	public:
		tur_unique<GraphicsRenderCommands> CreateGraphicsCommands() override
		{
			return tur::MakeUnique<gl::GraphicsRenderCommandsGL>(this);
		}

	public:
		void Present() override
		{
			platform::SwapBuffers(r_Window);
		}

	public:
		void DisplayVersion() override
		{
			gl::ShowOpenGLVersion();
		}

	public:
		inline gl::Renderpass GetRenderpass(RenderpassHandle handle) const
		{
			return m_Renderpasses[static_cast<uint32_t>(handle)];
		}

		inline gl::Buffer GetBuffer(BufferHandle handle) const
		{
			return m_Buffers[static_cast<uint32_t>(handle)];
		}

		inline gl::Shader GetShader(ShaderHandle handle) const
		{
			return m_Shaders[static_cast<uint32_t>(handle)];
		}

		inline gl::Pipeline GetPipeline(PipelineStateHandle handle) const
		{
			return m_Pipelines[static_cast<uint32_t>(handle)];
		}

	public:
		void CreateSwapchainRenderpass()
		{
			gl::Renderpass renderpass;

			RenderpassDescriptor desc = RenderpassDescriptor::CreateDefaultRenderpass();
			desc.extent = Extent {
				(uint32_t)r_Window->GetProperties().dimensions.x,
				(uint32_t)r_Window->GetProperties().dimensions.y,
			};

			SetupRenderpass(renderpass, desc);
			m_Renderpasses.push_back(renderpass);
		}

	private:
		std::vector<gl::Renderpass> m_Renderpasses;
		std::vector<gl::Buffer> m_Buffers;
		std::vector<gl::Shader> m_Shaders;
		std::vector<gl::Pipeline> m_Pipelines;

	private:
		NON_OWNING Window* r_Window = nullptr;
	};
}
