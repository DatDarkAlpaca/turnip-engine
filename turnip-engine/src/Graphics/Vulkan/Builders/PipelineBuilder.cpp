#include "pch.h"
#include "PipelineBuilder.h"
#include "Graphics/Vulkan/Converters.h"

namespace tur::vulkan
{
	Pipeline PipelineBuilder::Build()
    {
		if (!m_ArgumentsSet)
			TUR_LOG_CRITICAL("A valid device and swapchain must be provided to build a pipeline");

		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		// Vertex Input:
		std::vector<vk::VertexInputBindingDescription> bindings;
		std::vector<vk::VertexInputAttributeDescription> attributes;

		vk::VertexInputBindingDescription bindingDescription;
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = { };
		{
			vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();

			// TODO: input bindings, input rate, instance divisor
			// TODO: use vertex format stride
			{
				bindingDescription.binding = 0;
				bindingDescription.stride = descriptor.vertexFormat.attributes[0].stride;
				bindings.push_back(bindingDescription);
			}

			// TODO: link input bindings
			uint32_t offset = 0;
			for (const auto& attribute : descriptor.vertexFormat.attributes)
			{
				vk::VertexInputAttributeDescription attributeDescription;
				attributeDescription.binding = 0;
				attributeDescription.location = attribute.location;
				attributeDescription.format = GetFormat(attribute.layoutFormat);

				if (!attribute.offset)
					attributeDescription.offset = attribute.offset;
				else 
					attributeDescription.offset = offset;

				attributes.push_back(attributeDescription);
			}

			vertexInputInfo.pVertexBindingDescriptions = bindings.data();
			vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

			// TODO: flexible binding descriptions
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)descriptor.vertexFormat.attributes.size();
			
			pipelineInfo.pVertexInputState = &vertexInputInfo;
		}

		// Input Assembly:
		{
			vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { };
			inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
			inputAssemblyInfo.topology = GetTopology(descriptor.primitiveTopology);
			pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		}
	
		// Vertex Shader:
		vk::ShaderModule vertexModule = r_RenderDevice->GetShader(descriptor.vertexShader).shaderModule;
		{
			vk::PipelineShaderStageCreateInfo vertexShaderInfo = { };
			vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
			vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
			vertexShaderInfo.module = vertexModule;
			vertexShaderInfo.pName = "main";
			shaderStages.push_back(vertexShaderInfo);
		}

		// Tesselation Control Shader:
		vk::ShaderModule tessControlModule;
		if (descriptor.tesselationControlShader != ShaderHandle::INVALID)
		{
			tessControlModule = r_RenderDevice->GetShader(descriptor.tesselationControlShader).shaderModule;
			vk::PipelineShaderStageCreateInfo tesselationControlCreateInfo = { };
			tesselationControlCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			tesselationControlCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationControl;
			tesselationControlCreateInfo.module = tessControlModule;
			tesselationControlCreateInfo.pName = "main";
			shaderStages.push_back(tesselationControlCreateInfo);
		}

		// Tesselation Evaluation Shader:
		vk::ShaderModule tessEvalModule;
		if (descriptor.tesselationEvaluationShader != ShaderHandle::INVALID)
		{
			tessEvalModule = r_RenderDevice->GetShader(descriptor.tesselationEvaluationShader).shaderModule;
			vk::PipelineShaderStageCreateInfo tesselationEvalCreateInfo = { };
			tesselationEvalCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			tesselationEvalCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
			tesselationEvalCreateInfo.module = tessEvalModule;
			tesselationEvalCreateInfo.pName = "main";
			shaderStages.push_back(tesselationEvalCreateInfo);
		}

		// Geometry Shader:
		vk::ShaderModule geometryModule;
		if (descriptor.geometryShader != ShaderHandle::INVALID)
		{
			geometryModule = r_RenderDevice->GetShader(descriptor.geometryShader).shaderModule;
			vk::PipelineShaderStageCreateInfo geometryShaderCreateInfo = { };
			geometryShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			geometryShaderCreateInfo.stage = vk::ShaderStageFlagBits::eGeometry;
			geometryShaderCreateInfo.module = geometryModule;
			geometryShaderCreateInfo.pName = "main";
			shaderStages.push_back(geometryShaderCreateInfo);
		}
	
		// Viewports:
		std::vector<vk::Viewport> viewports;
		{
			viewports.reserve(descriptor.viewports.size());
			if (descriptor.viewports.empty())
			{
				Viewport viewport;
				{
					viewport.origin = glm::vec2(0.f);
					viewport.dimensions.x = (float)m_SwapchainExtent.width;
					viewport.dimensions.y = (float)m_SwapchainExtent.height;
					viewport.minDepth = 0.f;
					viewport.maxDepth = 1.f;
				}
				descriptor.viewports.push_back(viewport);
			}

			for (const auto& viewport : descriptor.viewports)
			{
				vk::Viewport entry;
				entry.width = viewport.dimensions.x;
				entry.height = viewport.dimensions.y;
				entry.minDepth = viewport.minDepth;
				entry.maxDepth = viewport.maxDepth;
				entry.x = viewport.origin.x;
				entry.y = viewport.origin.y;

				viewports.push_back(entry);
			}
		}
		
		// Scissors:
		std::vector<vk::Rect2D> scissors;
		{
			if (descriptor.scissors.empty())
			{
				Scissor scissor;
				{
					scissor.offset = glm::vec2(0);
					scissor.size.x = m_SwapchainExtent.width;
					scissor.size.y = m_SwapchainExtent.height;
				}
				descriptor.scissors.push_back(scissor);
			}

			scissors.reserve(descriptor.scissors.size());
			for (const auto& scissor : descriptor.scissors)
			{
				vk::Rect2D rect;
				{
					rect.offset = vk::Offset2D(scissor.offset.x, scissor.offset.y);
					rect.extent = vk::Extent2D(scissor.size.x, scissor.size.y);
				}

				scissors.push_back(rect);
			}
		}
		
		// Viewport State:
		{
			vk::PipelineViewportStateCreateInfo viewportState = { };
			viewportState.flags = vk::PipelineViewportStateCreateFlags();
			viewportState.viewportCount = static_cast<uint32_t>(viewports.size());
			viewportState.pViewports = viewports.data();
			viewportState.scissorCount = static_cast<uint32_t>(scissors.size());
			viewportState.pScissors = scissors.data();
			pipelineInfo.pViewportState = &viewportState;
		}
		
		// Dynamic Info:
		// TODO: provide dynamic scissor and viewports;
		/*{
			vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

			vk::PipelineDynamicStateCreateInfo dynamicInfo = { };
			dynamicInfo.flags = vk::PipelineDynamicStateCreateFlags();
			dynamicInfo.pDynamicStates = &dynamicStates[0];
			dynamicInfo.dynamicStateCount = 2;
			pipelineInfo.pDynamicState = &dynamicInfo;
		}*/

		// Rasterizer:
		{
			vk::PipelineRasterizationStateCreateInfo rasterizer = { };
			rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
			rasterizer.depthClampEnable = descriptor.clampDepth;
			rasterizer.rasterizerDiscardEnable = descriptor.discardFragments;
			rasterizer.polygonMode = GetPolygonMode(descriptor.polygonMode);
			rasterizer.lineWidth = descriptor.lineWidth;
			rasterizer.cullMode = GetCullMode(descriptor.cullMode);
			rasterizer.frontFace = GetFrontFace(descriptor.frontFace);
			rasterizer.depthBiasEnable = descriptor.enableDepthBias;
			pipelineInfo.pRasterizationState = &rasterizer;
		}
		
		// Fragment Shader:
		vk::ShaderModule fragmentModule = r_RenderDevice->GetShader(descriptor.fragmentShader).shaderModule;
		{
			vk::PipelineShaderStageCreateInfo fragmentShaderInfo = { };
			fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
			fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
			fragmentShaderInfo.module = fragmentModule;
			fragmentShaderInfo.pName = "main";
			shaderStages.push_back(fragmentShaderInfo);
		}
		
		// Pipeline Shader Stages:
		{
			pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
			pipelineInfo.pStages = shaderStages.data();
		}
		
		// Multisampling:
		{
			vk::PipelineMultisampleStateCreateInfo multisampling = { };
			multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
			multisampling.sampleShadingEnable = descriptor.enableMultisampling;
			multisampling.rasterizationSamples = GetSampleCount(descriptor.sampleCount);
			pipelineInfo.pMultisampleState = &multisampling;
		}
		
		// Color Blending:
		{
			vk::PipelineColorBlendAttachmentState colorBlendAttachment = { };

			colorBlendAttachment.colorWriteMask			= GetColorComponentFlags(descriptor.blendingWriteMask);
			colorBlendAttachment.blendEnable			= descriptor.enableColorBlending;
			colorBlendAttachment.srcColorBlendFactor	= GetBlendFactor(descriptor.srcColorBlendFactor, BlendFactor::ONE);
			colorBlendAttachment.dstColorBlendFactor	= GetBlendFactor(descriptor.dstColorBlendFactor, BlendFactor::ZERO);
			colorBlendAttachment.colorBlendOp			= GetBlendOperation(descriptor.colorBlendOp);
			colorBlendAttachment.srcAlphaBlendFactor	= GetBlendFactor(descriptor.srcAlphaColorBlendFactor, BlendFactor::ONE);
			colorBlendAttachment.dstAlphaBlendFactor	= GetBlendFactor(descriptor.dstAlphaColorBlendFactor, BlendFactor::ZERO);
			colorBlendAttachment.alphaBlendOp			= GetBlendOperation(descriptor.alphaBlendOp);
		
			vk::PipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
			colorBlending.logicOpEnable = descriptor.enableLogicOp;
			colorBlending.logicOp = GetLogicOp(descriptor.logicOperation);
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = descriptor.blendConstants[0];
			colorBlending.blendConstants[1] = descriptor.blendConstants[1];
			colorBlending.blendConstants[2] = descriptor.blendConstants[2];
			colorBlending.blendConstants[3] = descriptor.blendConstants[3];
			pipelineInfo.pColorBlendState = &colorBlending;
		}
		
		// Pipeline Layout:
		vk::PipelineLayout pipelineLayout = CreateLayout();
		{
			pipelineInfo.layout = pipelineLayout;
		}
		
		// Renderpass:
		{
			pipelineInfo.renderPass = m_Renderpass.renderpass;
			pipelineInfo.subpass = 0;

			pipelineInfo.basePipelineHandle = nullptr;
		}
		
		// Pipeline Creation:
		vk::Pipeline graphicsPipeline;
		try
		{
			// TODO: pipeline cache.
			graphicsPipeline = (m_Device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create pipeline: {}", err.what());
		}

		// Cleanup
		// TODO: remove shaders from renderdevice.
		m_Device.destroyShaderModule(vertexModule);

		if(tessControlModule)
			m_Device.destroyShaderModule(tessControlModule);

		if (tessEvalModule)
			m_Device.destroyShaderModule(tessEvalModule);

		if (geometryModule)
			m_Device.destroyShaderModule(geometryModule);

		m_Device.destroyShaderModule(fragmentModule);

		// Output:
		Pipeline pipelineResult;
		{
			pipelineResult.pipeline = graphicsPipeline;
			pipelineResult.layout = pipelineLayout;
		}

		return pipelineResult;
    }

	vk::PipelineLayout PipelineBuilder::CreateLayout() const
	{
		vk::PipelineLayoutCreateInfo layoutInfo = { };
		layoutInfo.flags = vk::PipelineLayoutCreateFlags();
		layoutInfo.setLayoutCount = descriptor.layoutCount;
		layoutInfo.pushConstantRangeCount = descriptor.pushConstantCount;

		try
		{
			return m_Device.createPipelineLayout(layoutInfo);
		}
		catch (const vk::SystemError& err)
		{
			TUR_LOG_ERROR("Failed to create pipeline layout: {}", err.what());
		}

		return nullptr;
	}

	PipelineBuilder& PipelineBuilder::SetArguments(const vk::Device& device, const Swapchain& swapchain, const RenderpassVulkan& renderpass)
	{
		m_Renderpass = renderpass;
		m_SwapchainFormat = swapchain.surfaceFormat.format;
		m_SwapchainExtent = swapchain.extent;
		m_Device = device;

		m_ArgumentsSet = true;

		return *this;
	}
}