#include "pch.hpp"
#include "pipeline_factory.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/objects/pipeline.hpp"

namespace tur::vulkan
{
	Pipeline create_graphics_pipeline(GraphicsDeviceVulkan& device, const PipelineDescriptor& descriptor)
	{
		Pipeline pipeline;
		pipeline.type = PipelineType::GRAPHICS;
		pipeline.descriptor = descriptor;

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

			dynamicStateCreateInfo.dynamicStateCount = static_cast<u32>(dynamicStates.size());
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

				pipelineViewportState.viewportCount = static_cast<u32>(viewports.size());
				pipelineViewportState.pViewports = viewports.data();
			}

			if (!descriptor.useDynamicScissor)
			{
				for (const auto& scissor : descriptor.scissors)
					viewports.push_back(vk::Viewport{ scissor.x, scissor.y, scissor.width, scissor.height });

				pipelineViewportState.viewportCount = static_cast<u32>(scissors.size());
				pipelineViewportState.pViewports = scissors.data();
			}
		}

		// Vertex Input:
		std::vector<vk::VertexInputBindingDescription>   bindingDescriptions;
		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
		{
			// Bindings:
			const auto& bindings = descriptor.vertexInputStage.bindings;		
			bindingDescriptions.reserve(bindings.size());
			for (const auto& binding : bindings)
			{
				vk::VertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = binding.binding;
				bindingDescription.stride = binding.stride;

				bool isVertexRate = binding.inputRate == InputRate::VERTEX;
				bindingDescription.inputRate = isVertexRate ? vk::VertexInputRate::eVertex : vk::VertexInputRate::eInstance;

				bindingDescriptions.push_back(bindingDescription);
			}

			// Attributes:
			const auto& attributes = descriptor.vertexInputStage.attributes;
			attributeDescriptions.reserve(attributes.size());
			for (const auto& attribute : attributes)
			{
				vk::VertexInputAttributeDescription attributeDescription = {};
				attributeDescription.binding = attribute.binding;
				attributeDescription.location = attribute.location;
				attributeDescription.format = get_attribute_format(attribute.format);
				attributeDescription.offset = attribute.offset;

				attributeDescriptions.push_back(attributeDescription);
			}
		}

		vk::PipelineVertexInputStateCreateInfo inputCreateInfo = {};
		{
			inputCreateInfo.flags = vk::PipelineVertexInputStateCreateFlags();
			
			inputCreateInfo.vertexBindingDescriptionCount = static_cast<u32>(bindingDescriptions.size());
			inputCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();

			inputCreateInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
			inputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		}

		// Input Assembly:
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
		{
			inputAssemblyCreateInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
			inputAssemblyCreateInfo.primitiveRestartEnable = descriptor.inputAssemblyStage.primitiveRestartEnable;
			inputAssemblyCreateInfo.topology = get_primitive_topology(descriptor.inputAssemblyStage.topology);
		}

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
			colorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eOne;
			colorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eZero;

			colorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;

			colorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eOne;
			colorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eZero;

			colorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
			colorBlendAttachmentState.colorWriteMask = colorComponentFlags;
		}

		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
		{
			pipelineColorBlendStateCreateInfo.flags = vk::PipelineColorBlendStateCreateFlags();
			pipelineColorBlendStateCreateInfo.logicOpEnable = false;
			pipelineColorBlendStateCreateInfo.logicOp = vk::LogicOp::eCopy;
			pipelineColorBlendStateCreateInfo.attachmentCount = 1;
			pipelineColorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
			pipelineColorBlendStateCreateInfo.blendConstants = { { 0.0f, 0.0f, 0.0f, 0.0f } };
		}
	
		// Pipeline Layout:
		{
			const auto& bindingDescriptors = descriptor.pipelineLayout.bindingDescriptors;
			auto& state = device.get_state();

			std::vector<vk::DescriptorSetLayoutBinding> descriptorBindings;
			for (const auto& [binding, type, stages, amount] : bindingDescriptors)
			{
				vk::DescriptorSetLayoutBinding descriptorBinding = {};
				descriptorBinding.binding = binding;
				descriptorBinding.descriptorType = get_descriptor_type(type);
				descriptorBinding.stageFlags = get_pipeline_stages(stages);
				descriptorBinding.descriptorCount = amount;

				descriptorBindings.push_back(descriptorBinding);
			}

			vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
			descriptorSetLayoutInfo.bindingCount = static_cast<u32>(descriptorBindings.size());
			descriptorSetLayoutInfo.pBindings = descriptorBindings.data();

			state.descriptorSetLayout = state.logicalDevice.createDescriptorSetLayout(descriptorSetLayoutInfo);

			vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			{
				pipelineLayoutCreateInfo.flags = vk::PipelineLayoutCreateFlags();
				pipelineLayoutCreateInfo.setLayoutCount = 1;
				pipelineLayoutCreateInfo.pSetLayouts = &state.descriptorSetLayout;
				pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
				pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
			}

			try
			{
				pipeline.layout = device.get_state().logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
			}
			catch (vk::SystemError& err)
			{
				TUR_LOG_CRITICAL("Failed to create pipeline layout. {}", err.what());
			}
		}
		
		// Descriptor Pool:
		const u32 frameAmount = device.get_state().frameDataHolder.get_frames().size();
		{
			const auto& bindingDescriptors = descriptor.pipelineLayout.bindingDescriptors;
			std::vector<vk::DescriptorPoolSize> poolSizes;

			for (const auto& bindingDescriptor : bindingDescriptors)
			{
				vk::DescriptorPoolSize poolSize;
				poolSize.type = get_descriptor_type(bindingDescriptor.type);
				poolSize.descriptorCount = bindingDescriptor.amount;
				poolSizes.push_back(poolSize);
			}

			vk::DescriptorPoolCreateInfo poolInfo;
			{
				poolInfo.flags = vk::DescriptorPoolCreateFlags();
				poolInfo.maxSets = frameAmount;
				poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
				poolInfo.pPoolSizes = poolSizes.data();
			}

			try 
			{
				device.get_state().descriptorPool = logicalDevice.createDescriptorPool(poolInfo);
			}
			catch (vk::SystemError err) 
			{
				TUR_LOG_CRITICAL("Failed to create descriptor pool");
			}
		}

		// Pipeline:
		// ! No renderpass since the vulkan device will use dynamic rendering.
		vk::GraphicsPipelineCreateInfo pipelineInfo;
		{
			pipelineInfo.flags = vk::PipelineCreateFlags();

			pipelineInfo.stageCount = static_cast<u32>(shaderCreateInfos.size());
			pipelineInfo.pStages = shaderCreateInfos.data();

			pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
			pipelineInfo.pViewportState = &pipelineViewportState;

			pipelineInfo.pVertexInputState = &inputCreateInfo;
			pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
			pipelineInfo.pRasterizationState = &rasterizerCreateInfo;
			pipelineInfo.pMultisampleState = &multisampleCreateInfo;
			pipelineInfo.pDepthStencilState = nullptr;
			pipelineInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;

			pipelineInfo.layout = pipeline.layout;
		}

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

			pipeline.pipeline = result.value;
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to create graphics pipeline. {}", err.what());
		}
		
		return pipeline;
	}
}