#include "pch.hpp"
#include "pipeline_factory.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/objects/pipeline.hpp"

namespace tur::vulkan
{
	vk::Pipeline tur::vulkan::create_graphics_pipeline(GraphicsDeviceVulkan& device, const PipelineDescriptor& descriptor)
	{
		auto& logicalDevice = device.get_state().logicalDevice;
		auto& shaders = device.get_shader_modules();

		// Shaders:
		std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos = {};
		{
			// Vertex:
			vk::PipelineShaderStageCreateInfo vertexShaderCreateInfo;
			{
				vertexShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
				vertexShaderCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;
				vertexShaderCreateInfo.pName = "main";
				vertexShaderCreateInfo.module = shaders.get(descriptor.vertexShader);
			}
			shaderCreateInfos.push_back(vertexShaderCreateInfo);

			if (descriptor.tesselationControlShader != invalid_handle)
			{
				vk::PipelineShaderStageCreateInfo tesselationControlShaderCreateInfo;
				{
					tesselationControlShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
					tesselationControlShaderCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationControl;
					tesselationControlShaderCreateInfo.pName = "main";
					tesselationControlShaderCreateInfo.module = shaders.get(descriptor.tesselationControlShader);
				}
				shaderCreateInfos.push_back(tesselationControlShaderCreateInfo);
			}

			if (descriptor.tesselationEvaluationShader != invalid_handle)
			{
				vk::PipelineShaderStageCreateInfo tesselationEvalShaderCreateInfo;
				{
					tesselationEvalShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
					tesselationEvalShaderCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
					tesselationEvalShaderCreateInfo.pName = "main";
					tesselationEvalShaderCreateInfo.module = shaders.get(descriptor.tesselationEvaluationShader);
				}
				shaderCreateInfos.push_back(tesselationEvalShaderCreateInfo);
			}

			if (descriptor.geometryShader != invalid_handle)
			{
				vk::PipelineShaderStageCreateInfo geometryShaderCreateInfo;
				{
					geometryShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
					geometryShaderCreateInfo.stage = vk::ShaderStageFlagBits::eGeometry;
					geometryShaderCreateInfo.pName = "main";
					geometryShaderCreateInfo.module = shaders.get(descriptor.geometryShader);
				}
				shaderCreateInfos.push_back(geometryShaderCreateInfo);
			}

			// Fragment:
			vk::PipelineShaderStageCreateInfo fragmentShaderCreateInfo;
			{
				fragmentShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
				fragmentShaderCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;
				fragmentShaderCreateInfo.pName = "main";
				fragmentShaderCreateInfo.module = shaders.get(descriptor.fragmentShader);
			}
			shaderCreateInfos.push_back(fragmentShaderCreateInfo);
		}

		// Dynamic State:
		std::vector<vk::DynamicState> dynamicStates;
		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		{
			dynamicStateCreateInfo.flags = vk::PipelineDynamicStateCreateFlags();

			if (descriptor.useDynamicViewport)
				dynamicStates.push_back(vk::DynamicState::eViewport);

			if (descriptor.useDynamicScissor)
				dynamicStates.push_back(vk::DynamicState::eScissor);

			dynamicStateCreateInfo.dynamicStateCount = dynamicStates.size();
			dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();
		}

		// Viewport & Scissor:
		vk::PipelineViewportStateCreateInfo pipelineViewportState = {};
		{
			pipelineViewportState.flags = vk::PipelineViewportStateCreateFlags();
			pipelineViewportState.scissorCount = descriptor.scissorCount;
			pipelineViewportState.viewportCount = descriptor.viewportCount;

			std::vector<vk::Viewport> viewports;
			std::vector<vk::Viewport> scissors;
			if (!descriptor.useDynamicViewport)
			{
				for (const auto& viewport : descriptor.viewports)
					viewports.push_back(vk::Viewport{ viewport.x, viewport.y, viewport.width, viewport.height });

				pipelineViewportState.viewportCount = viewports.size();
				pipelineViewportState.pViewports = viewports.data();
			}

			if (!descriptor.useDynamicScissor)
			{
				for (const auto& scissor : descriptor.scissors)
					viewports.push_back(vk::Viewport{ scissor.x, scissor.y, scissor.width, scissor.height });

				pipelineViewportState.viewportCount = scissors.size();
				pipelineViewportState.pViewports = scissors.data();
			}
		}

		// Vertex Input:
		// TODO: use descriptors.
		vk::PipelineVertexInputStateCreateInfo inputCreateInfo = {};
		{
			inputCreateInfo.flags = vk::PipelineVertexInputStateCreateFlags();
			
			inputCreateInfo.vertexBindingDescriptionCount = 0;
			inputCreateInfo.pVertexBindingDescriptions = nullptr;

			inputCreateInfo.vertexAttributeDescriptionCount = 0;
			inputCreateInfo.pVertexAttributeDescriptions = nullptr;
		}

		// Input Assembly:
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
		{
			inputAssemblyCreateInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
			inputAssemblyCreateInfo.primitiveRestartEnable = descriptor.inputAssemblyStage.primitiveRestartEnable;
			inputAssemblyCreateInfo.topology = get_primitive_topology(descriptor.inputAssemblyStage.topology);
		}

		// polygonMode
		// cullMode
		// frontFace
		// depthBiasEnable
		// depthBiasConstantFactor
		// depthBiasClamp
		// depthBiasSlopeFactor
		// lineWidth

		// Rasterizer:
		vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
		{
			auto& rasterizer = descriptor.rasterizerStage;

			rasterizerCreateInfo.flags = vk::PipelineRasterizationStateCreateFlags();
			rasterizerCreateInfo.depthClampEnable = rasterizer.clampDepth;
			rasterizerCreateInfo.rasterizerDiscardEnable = rasterizer.discardFragments;

			rasterizerCreateInfo.cullMode = get_cull_mode(rasterizer.cullMode);
			rasterizerCreateInfo.frontFace = get_front_face(rasterizer.frontFace);
			rasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
			rasterizerCreateInfo.depthBiasClamp = 0.0f;
			rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;
			
			rasterizerCreateInfo.depthBiasEnable = rasterizer.enableDepthBias;
			rasterizerCreateInfo.lineWidth = rasterizer.lineWidth;
		}

		// Multisampling:
		vk::PipelineMultisampleStateCreateInfo multisampleCreateInfo = {};
		{
			// TODO: descriptor.
			multisampleCreateInfo.flags = vk::PipelineMultisampleStateCreateFlags();
			multisampleCreateInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
		}

		// Depth & Stencil
		{
			// TODO: implement
		}

		// Color blending:
		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState = {};
		{
			// TODO: descriptor

			vk::ColorComponentFlags colorComponentFlags
			(
				vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | 
				vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
			);

			colorBlendAttachmentState.blendEnable = false;
			colorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eZero;
			colorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eZero;

			colorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;

			colorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eZero;
			colorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eZero;

			colorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
			colorBlendAttachmentState.colorWriteMask = colorComponentFlags;
		}

		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
		{
			pipelineColorBlendStateCreateInfo.flags = vk::PipelineColorBlendStateCreateFlags();
			pipelineColorBlendStateCreateInfo.logicOpEnable = false;
			pipelineColorBlendStateCreateInfo.logicOp = vk::LogicOp::eNoOp;
			pipelineColorBlendStateCreateInfo.attachmentCount = 1;
			pipelineColorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
			pipelineColorBlendStateCreateInfo.blendConstants = { { 1.0f, 1.0f, 1.0f, 1.0f } };
		}
	
		// Pipeline Layout:
		vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		{
			// TODO: descriptor.

			pipelineLayoutCreateInfo.flags = vk::PipelineLayoutCreateFlags();
			pipelineLayoutCreateInfo.setLayoutCount = 0;
			pipelineLayoutCreateInfo.pSetLayouts = nullptr;
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		}

		vk::PipelineLayout pipelineLayout;
		try
		{
			pipelineLayout = device.get_state().logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
		} 
		catch(vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to create pipeline layout. {}", err.what());
		}

		// Pipeline:
		// ! No renderpass since the vulkan device will use dynamic rendering.
		vk::GraphicsPipelineCreateInfo pipelineInfo;
		{
			pipelineInfo.flags = vk::PipelineCreateFlags();

			pipelineInfo.stageCount = shaderCreateInfos.size();
			pipelineInfo.pStages = shaderCreateInfos.data();

			pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineInfo.pViewportState = &pipelineViewportState;

			pipelineInfo.pVertexInputState = &inputCreateInfo;
			pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
			pipelineInfo.pRasterizationState = &rasterizerCreateInfo;
			pipelineInfo.pMultisampleState = &multisampleCreateInfo;
			pipelineInfo.pDepthStencilState = nullptr;
			pipelineInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;

			pipelineInfo.layout = pipelineLayout;
		}

		vk::Pipeline graphicsPipeline;
		try 
		{
			auto result = device.get_state().logicalDevice.createGraphicsPipeline(nullptr, pipelineInfo);
			switch (result.result)
			{
			case vk::Result::eSuccess:
				break;

			case vk::Result::ePipelineCompileRequiredEXT:
				TUR_LOG_CRITICAL("VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT_EXT on PipelineCreateInfo");
				break;

			default:
				TUR_LOG_CRITICAL("Pipeline creation gone wild");
			}

			graphicsPipeline = result.value;
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to create graphics pipeline. {}", err.what());
		}
		
		return graphicsPipeline;
	}
}