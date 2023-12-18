#include "pch.h"
#include "PipelineVulkan.h"

namespace tur
{
	static constexpr vk::PrimitiveTopology GetTopology(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::POINTS:
				return vk::PrimitiveTopology::ePointList;

			case PrimitiveTopology::LINES:
				return vk::PrimitiveTopology::eLineList;

			case PrimitiveTopology::LINE_STRIPS:
				return vk::PrimitiveTopology::eLineStrip;

			case PrimitiveTopology::TRIANGLES:
				return vk::PrimitiveTopology::eTriangleList;

			case PrimitiveTopology::TRAIANGLE_STRIPS:
				return vk::PrimitiveTopology::eTriangleStrip;

			case PrimitiveTopology::TRIANGLE_FANS:
				return vk::PrimitiveTopology::eTriangleFan;

			case PrimitiveTopology::LINES_WITH_ADJACENCY:
				return vk::PrimitiveTopology::eLineListWithAdjacency;

			case PrimitiveTopology::LINE_STRIPS_WITH_ADJACENCY:
				return vk::PrimitiveTopology::eLineStripWithAdjacency;

			case PrimitiveTopology::TRIANGLES_WITH_ADJACENCY:
				return vk::PrimitiveTopology::eTriangleListWithAdjacency;

			case PrimitiveTopology::TRIANGLE_STRIPS_WITH_ADJACENCY:
				return vk::PrimitiveTopology::eTriangleStripWithAdjacency;

			case PrimitiveTopology::PATCHES:
				return vk::PrimitiveTopology::ePatchList;

			default:
				TUR_LOG_CRITICAL("Invalid topology");
		}
	}

	static constexpr vk::PolygonMode GetPolygonMode(PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
			case PolygonMode::FILL:
				return vk::PolygonMode::eFill;

			case PolygonMode::LINE:
				return vk::PolygonMode::eLine;

			case PolygonMode::POINT:
				return vk::PolygonMode::ePoint;

			case PolygonMode::FILL_RECTANGLE:
				return vk::PolygonMode::eFillRectangleNV;

			default:
				TUR_LOG_CRITICAL("Invalid Polygon Mode");
		}
	}

	static constexpr vk::CullModeFlags GetCullMode(CullMode cullMode)
	{
		switch (cullMode)
		{
			case CullMode::FRONT:
				return vk::CullModeFlagBits::eFront;

			case CullMode::BACK:
				return vk::CullModeFlagBits::eBack;

			case CullMode::FRONT_AND_BACK:
				return vk::CullModeFlagBits::eFrontAndBack;

			default:
				TUR_LOG_CRITICAL("Invalid CullMode");
		}
	}

	static constexpr vk::FrontFace GetFrontFace(FrontFace frontFace)
	{
		switch (frontFace)
		{
			case FrontFace::COUNTER_CLOCKWISE:
				return vk::FrontFace::eCounterClockwise;

			case FrontFace::CLOCKWISE:
				return vk::FrontFace::eClockwise;

			default:
				TUR_LOG_CRITICAL("Invalid Front Face");
		}
	}

	static constexpr vk::SampleCountFlagBits GetSampleCount(uint32_t samples)
	{
		switch (samples)
		{
			case 1:
				return vk::SampleCountFlagBits::e1;
			case 2:
				return vk::SampleCountFlagBits::e2;
			case 4:
				return vk::SampleCountFlagBits::e4;
			case 8:
				return vk::SampleCountFlagBits::e8;
			case 16:
				return vk::SampleCountFlagBits::e16;
			case 32:
				return vk::SampleCountFlagBits::e32;
			case 64:
				return vk::SampleCountFlagBits::e64;
			default:
			{
				TUR_LOG_ERROR("Multisampler samples count must be a power of 2. Setting value to 1");
				return vk::SampleCountFlagBits::e1;
			} break;
		}
	}

	static inline vk::ColorComponentFlags GetColorComponentFlags(ColorComponents components) 
	{
		vk::ColorComponentFlags flags;

		if (uint32_t(components & ColorComponents::R))
			flags |= vk::ColorComponentFlagBits::eR;

		if (uint32_t(components & ColorComponents::G))
			flags |= vk::ColorComponentFlagBits::eG;

		if (uint32_t(components & ColorComponents::B))
			flags |= vk::ColorComponentFlagBits::eB;

		if (uint32_t(components & ColorComponents::A))
			flags |= vk::ColorComponentFlagBits::eA;

		return flags;
	}

	static constexpr vk::LogicOp GetLogicOp(ColorBlendingOperation operation)
	{
		switch (operation)
		{
			case ColorBlendingOperation::CLEAR:
				return vk::LogicOp::eClear;

			case ColorBlendingOperation::AND:
				return vk::LogicOp::eAnd;

			case ColorBlendingOperation::AND_REVERSE:
				return vk::LogicOp::eAndReverse;

			case ColorBlendingOperation::COPY:
				return vk::LogicOp::eCopy;

			case ColorBlendingOperation::AND_INVERTED:
				return vk::LogicOp::eAndInverted;

			case ColorBlendingOperation::NO_OP:
				return vk::LogicOp::eNoOp;

			case ColorBlendingOperation::XOR:
				return vk::LogicOp::eXor;

			case ColorBlendingOperation::OR:
				return vk::LogicOp::eOr;

			case ColorBlendingOperation::NOR:
				return vk::LogicOp::eNor;

			case ColorBlendingOperation::EQUIVALENT:
				return vk::LogicOp::eEquivalent;

			case ColorBlendingOperation::INVERT:
				return vk::LogicOp::eInvert;

			case ColorBlendingOperation::OR_REVERSE:
				return vk::LogicOp::eOrReverse;

			case ColorBlendingOperation::COPY_INVERTED:
				return vk::LogicOp::eCopyInverted;

			case ColorBlendingOperation::OR_INVERTED:
				return vk::LogicOp::eOrInverted;

			case ColorBlendingOperation::NAND:
				return vk::LogicOp::eNand;

			case ColorBlendingOperation::SET:
				return vk::LogicOp::eSet;

			default:
				TUR_LOG_CRITICAL("Invalid Color Blending Operation");
		}
	}
}

namespace tur
{
    PipelineVulkan tur::PipelineBuilder::Build()
    {
		if (!m_DeviceSet)
			TUR_LOG_CRITICAL("A valid device must be provided to build a pipeline");

		if (!m_SwapchainSet)
			TUR_LOG_CRITICAL("A valid swapchain must be provided to build a pipeline");

		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		// Vertex Input:
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = { };
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		vertexInputInfo.vertexBindingDescriptionCount = descriptor.vertexBindingDescriptionCount;
		vertexInputInfo.vertexAttributeDescriptionCount = descriptor.vertexAttributeDescriptionCount;
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		// Input Assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { };
		inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssemblyInfo.topology = GetTopology(descriptor.topology);
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

		// Vertex Shader:
		vk::ShaderModule vertexModule = static_cast<ShaderVulkan*>(descriptor.vertexShader)->GetModule();
		vk::PipelineShaderStageCreateInfo vertexShaderInfo = { };
		vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertexShaderInfo.module = vertexModule;
		vertexShaderInfo.pName = "main";
		shaderStages.push_back(vertexShaderInfo);

		// Tesselation Control Shader:
		vk::ShaderModule tessControlModule;
		if (descriptor.tesselationControlShader)
		{
			tessControlModule = static_cast<ShaderVulkan*>(descriptor.tesselationControlShader)->GetModule();
			vk::PipelineShaderStageCreateInfo tesselationControlCreateInfo = { };
			tesselationControlCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			tesselationControlCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationControl;
			tesselationControlCreateInfo.module = tessControlModule;
			tesselationControlCreateInfo.pName = "main";
			shaderStages.push_back(tesselationControlCreateInfo);
		}

		// Tesselation Evaluation Shader:
		vk::ShaderModule tessEvalModule;
		if (descriptor.tesselationEvaluationShader)
		{
			tessEvalModule = static_cast<ShaderVulkan*>(descriptor.tesselationEvaluationShader)->GetModule();
			vk::PipelineShaderStageCreateInfo tesselationEvalCreateInfo = { };
			tesselationEvalCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			tesselationEvalCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
			tesselationEvalCreateInfo.module = tessEvalModule;
			tesselationEvalCreateInfo.pName = "main";
			shaderStages.push_back(tesselationEvalCreateInfo);
		}

		// Geometry Shader:
		vk::ShaderModule geometryModule;
		if (descriptor.geometryShader)
		{
			geometryModule = static_cast<ShaderVulkan*>(descriptor.geometryShader)->GetModule();
			vk::PipelineShaderStageCreateInfo geometryShaderCreateInfo = { };
			geometryShaderCreateInfo.flags = vk::PipelineShaderStageCreateFlags();
			geometryShaderCreateInfo.stage = vk::ShaderStageFlagBits::eGeometry;
			geometryShaderCreateInfo.module = geometryModule;
			geometryShaderCreateInfo.pName = "main";
			shaderStages.push_back(geometryShaderCreateInfo);
		}
	
		// Viewports:
		std::vector<vk::Viewport> viewports;
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
		
		// Scissors:
		if (descriptor.scissors.empty())
		{
			Scissor scissor;
			{
				scissor.offset = glm::vec2(0.f);
				scissor.size.x = m_SwapchainExtent.width;
				scissor.size.y = m_SwapchainExtent.height;
			}
			descriptor.scissors.push_back(scissor);
		}

		std::vector<vk::Rect2D> scissors;
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

		// Viewport State:
		vk::PipelineViewportStateCreateInfo viewportState = { };
		viewportState.flags = vk::PipelineViewportStateCreateFlags();
		viewportState.viewportCount = viewports.size();
		viewportState.pViewports = viewports.data();
		viewportState.scissorCount = scissors.size();
		viewportState.pScissors = scissors.data();
		pipelineInfo.pViewportState = &viewportState;

		// Rasterizer
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

		// Fragment Shader:
		vk::ShaderModule fragmentModule = static_cast<ShaderVulkan*>(descriptor.fragmentShader)->GetModule();
		vk::PipelineShaderStageCreateInfo fragmentShaderInfo = { };
		fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragmentShaderInfo.module = fragmentModule;
		fragmentShaderInfo.pName = "main";
		shaderStages.push_back(fragmentShaderInfo);

		// Pipeline Shader Stages:
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();

		// Multisampling
		vk::PipelineMultisampleStateCreateInfo multisampling = { };
		multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
		multisampling.sampleShadingEnable = descriptor.enableMultisampling;
		multisampling.rasterizationSamples = GetSampleCount(descriptor.sampleCount);
		pipelineInfo.pMultisampleState = &multisampling;

		// Color Blending:
		vk::PipelineColorBlendAttachmentState colorBlendAttachment = { };
		colorBlendAttachment.colorWriteMask = GetColorComponentFlags(descriptor.blendingWriteMask);
		colorBlendAttachment.blendEnable = descriptor.enableColorBlending;

		vk::PipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
		colorBlending.logicOpEnable = descriptor.enableLogicOp;
		colorBlending.logicOp = GetLogicOp(descriptor.blendingOperation);
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = descriptor.blendConstants[0];
		colorBlending.blendConstants[1] = descriptor.blendConstants[1];
		colorBlending.blendConstants[2] = descriptor.blendConstants[2];
		colorBlending.blendConstants[3] = descriptor.blendConstants[3];
		pipelineInfo.pColorBlendState = &colorBlending;

		// Pipeline Layout:
		vk::PipelineLayout pipelineLayout = CreateLayout();
		pipelineInfo.layout = pipelineLayout;

		// Renderpass:
		vk::RenderPass renderpass = CreateRenderpass();
		pipelineInfo.renderPass = renderpass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = nullptr;

		// Pipeline Creation:
		vk::Pipeline graphicsPipeline;
		try
		{
			// TODO: pipeline cache.
			graphicsPipeline = (m_Device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_ERROR("Failed to create pipeline: {}", err.what());
		}

		// Cleanup
		m_Device.destroyShaderModule(vertexModule);
		if(tessControlModule)
			m_Device.destroyShaderModule(tessControlModule);
		if (tessEvalModule)
			m_Device.destroyShaderModule(tessEvalModule);
		if (geometryModule)
			m_Device.destroyShaderModule(geometryModule);
		m_Device.destroyShaderModule(fragmentModule);

		// Output:
		PipelineVulkan pipelineResult;
		{
			pipelineResult.pipeline = graphicsPipeline;
			pipelineResult.layout = pipelineLayout;
			pipelineResult.renderpass = renderpass;
		}

		return pipelineResult;
    }
}