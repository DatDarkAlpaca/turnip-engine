#pragma once
#include <glad/glad.h>

#include "Rendering/RenderDevice.h"
#include "Util/File.h"

#include "BufferGL.h"
#include "ShaderGL.h"
#include "PipelineGL.h"

namespace tur
{
	class RenderDeviceGL : public RenderDevice
	{
	public:
		BufferHandle CreateBuffer(const BufferDescriptor& bufferDescription) override
		{
			uint32_t target = gl::TranslateBindingFlag(bufferDescription.bindingFlag);
			uint32_t usage = gl::TranslateUsageFlag(bufferDescription.usageFlag);

			gl::BufferGL buffer;
			glGenBuffers(1, &buffer.id);
			glBindBuffer(target, buffer.id);
			glBufferData(target, bufferDescription.dataSize, bufferDescription.data, usage);

			m_Buffers.push_back(buffer);
			return BufferHandle(m_Buffers.size() - 1);
		}

		ShaderHandle CreateShader(const ShaderDescriptor& shaderDescriptor) override
		{
			auto shaderContents = ReadFile(shaderDescriptor.filepath);
			const char* cShaderCode = shaderContents.c_str();

			gl::ShaderGL shader;
			shader.id = glCreateShader(gl::GetShaderTypeValue(shaderDescriptor.type));

			glShaderSource(shader.id, 1, &cShaderCode, NULL);
			glCompileShader(shader.id);

			gl::CheckCompileErrors(shader.id, shaderDescriptor.type);

			m_Shaders.push_back(shader);
			return ShaderHandle(m_Shaders.size() - 1);
		}

		PipelineStateHandle CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor) override
		{
			std::vector<gl::ShaderGL> shaders;
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

			gl::PipelineGL pipeline;
			pipeline.state = pipelineDescriptor;

			SetupPipelineShaders(pipeline, shaders);

			m_Pipelines.push_back(pipeline);
			return PipelineStateHandle(m_Pipelines.size() - 1);
		}

	public:
		inline gl::BufferGL GetBuffer(BufferHandle handle) const
		{
			return m_Buffers[static_cast<uint32_t>(handle)];
		}

		inline gl::ShaderGL GetShader(ShaderHandle handle) const
		{
			return m_Shaders[static_cast<uint32_t>(handle)];
		}

		inline gl::PipelineGL GetPipeline(PipelineStateHandle handle) const
		{
			return m_Pipelines[static_cast<uint32_t>(handle)];
		}

	private:
		std::vector<gl::BufferGL> m_Buffers;
		std::vector<gl::ShaderGL> m_Shaders;
		std::vector<gl::PipelineGL> m_Pipelines;
	};
}
