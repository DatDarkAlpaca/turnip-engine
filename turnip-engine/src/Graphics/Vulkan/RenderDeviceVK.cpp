#include "pch.h"
#include "Util/File.h"
#include "RenderDeviceVK.h"

#include "Builders/FramebufferBuilder.h"
#include "Builders/RenderpassBuilder.h"
#include "Builders/PipelineBuilder.h"

namespace tur::vulkan
{
	RenderDeviceVK::RenderDeviceVK(NON_OWNING Window* window)
		: r_Window(window)
	{

	}

	RenderpassHandle RenderDeviceVK::CreateRenderpass(const RenderpassDescriptor& renderpassDescription)
	{
		RenderpassBuilder builder(renderpassDescription);
		auto renderpass = builder.SetArguments(logicalDevice, swapchain)
						         .Build();

		if (!renderpass.has_value())
		{
			TUR_LOG_WARN("Attempted to create renderpass with invalid arguments.");
			return RenderpassHandle::INVALID;
		}

		auto renderpassData = renderpass.value();

		m_Renderpasses.push_back(renderpassData);		
		return RenderpassHandle(m_Renderpasses.size() - 1);
	}

	BufferHandle RenderDeviceVK::CreateBuffer(const BufferDescriptor& bufferDescription)
	{
		return BufferHandle::INVALID;
	}

	ShaderHandle RenderDeviceVK::CreateShader(const ShaderDescriptor& shaderDescriptor)
	{
		auto sourceCode = ReadBinaryFile(shaderDescriptor.filepath);

		vk::ShaderModuleCreateInfo moduleInfo = {};
		moduleInfo.flags = vk::ShaderModuleCreateFlags();
		moduleInfo.codeSize = sourceCode.size();
		moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

		vulkan::Shader shader;

		try
		{
			shader.shaderModule = logicalDevice.createShaderModule(moduleInfo);
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_ERROR("Failed to create shader module from filepath: {}. {}", shaderDescriptor.filepath, err.what());
		}

		m_Shaders.push_back(shader);
		return ShaderHandle(m_Shaders.size() - 1);
	}

	PipelineStateHandle RenderDeviceVK::CreatePipeline(const PipelineStateDescriptor& pipelineDescriptor)
	{
		// Default Renderpass:
		vulkan::RenderpassVulkan renderpass;
		if (pipelineDescriptor.renderpass == RenderpassHandle::INVALID)
			renderpass = m_Renderpasses[0];
		else
			renderpass = m_Renderpasses[(uint32_t)pipelineDescriptor.renderpass];

		vulkan::PipelineBuilder builder(this, pipelineDescriptor);
		builder.SetArguments(logicalDevice, swapchain, renderpass);

		vulkan::Pipeline pipeline = builder.Build();
		m_Pipelines.push_back(pipeline);

		return PipelineStateHandle(m_Pipelines.size() - 1);
	}

	void RenderDeviceVK::FinishSetup()
	{
		// Default renderpass:
		vulkan::RenderpassVulkan renderpass;

		vulkan::RenderpassBuilder renderpassBuilder(RenderpassDescriptor::CreateDefaultRenderpass());
		renderpassBuilder.SetArguments(logicalDevice, swapchain);
		renderpass = renderpassBuilder.Build().value();
		m_Renderpasses.push_back(renderpass);

		// Framebuffers:
		vulkan::FramebufferBuilder frameBuilder;
		frameBuilder.SetArguments(logicalDevice, renderpass);

		for (auto& frame : swapchain.frames)
			frame.framebuffer = frameBuilder.Create(frame.view, swapchain);
	}

	Barrier RenderDeviceVK::Submit(RenderCommands* context)
	{
		return Barrier();
	}

	void RenderDeviceVK::WaitBarrier(const Barrier& barrier)
	{
	}

	void RenderDeviceVK::Present()
	{
	}
}