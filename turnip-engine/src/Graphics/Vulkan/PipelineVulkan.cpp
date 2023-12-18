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
			{
				TUR_LOG_ERROR("Invalid topology. Using default: TRIANGLES");
				return vk::PrimitiveTopology::eTriangleList;
			}
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
			{
				TUR_LOG_ERROR("Invalid Polygon Mode. Using Default: FILL");
				return vk::PolygonMode::eFill;
			}
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
			{
				TUR_LOG_ERROR("Invalid Cull Mode. Using default: BACK");
				return vk::CullModeFlagBits::eBack;
			}
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
			{
				TUR_LOG_ERROR("Invalid Front Face. Using default: CLOCKWISE");
				return vk::FrontFace::eClockwise;
			} break;
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
				TUR_LOG_ERROR("Multisampler samples count must be a power of 2. Using Default: 1");
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

	static constexpr vk::BlendFactor GetBlendFactor(BlendFactor blendFactor, BlendFactor fallback)
	{
		switch (blendFactor)
		{
			case BlendFactor::ZERO:
				return vk::BlendFactor::eZero;
			case BlendFactor::ONE:
				return vk::BlendFactor::eOne;
			case BlendFactor::SRC_COLOR:
				return vk::BlendFactor::eSrcColor;
			case BlendFactor::ONE_MINUS_SRC_COLOR:
				return vk::BlendFactor::eOneMinusSrcColor;
			case BlendFactor::DST_COLOR:
				return vk::BlendFactor::eDstColor;
			case BlendFactor::ONE_MINUS_DST_COLOR:
				return vk::BlendFactor::eOneMinusDstColor;
			case BlendFactor::SRC_ALPHA:
				return vk::BlendFactor::eSrcAlpha;
			case BlendFactor::ONE_MINUS_SRC_ALPHA:
				return vk::BlendFactor::eOneMinusSrcAlpha;
			case BlendFactor::DST_ALPHA:
				return vk::BlendFactor::eDstAlpha;
			case BlendFactor::ONE_MINUS_DST_ALPHA:
				return vk::BlendFactor::eOneMinusDstAlpha;
			case BlendFactor::CONSTANT_COLOR:
				return vk::BlendFactor::eConstantColor;
			case BlendFactor::ONE_MINUS_CONSTANT_COLOR:
				return vk::BlendFactor::eOneMinusConstantColor;
			case BlendFactor::CONSTANT_ALPHA:
				return vk::BlendFactor::eConstantAlpha;
			case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
				return vk::BlendFactor::eOneMinusConstantAlpha;
			case BlendFactor::SRC_ALPHA_SATURATE:
				return vk::BlendFactor::eSrcAlphaSaturate;
			case BlendFactor::SRC1_COLOR:
				return vk::BlendFactor::eSrc1Color;
			case BlendFactor::ONE_MINUS_SRC1_COLOR:
				return vk::BlendFactor::eOneMinusSrc1Color;
			case BlendFactor::SRC1_ALPHA:
				return vk::BlendFactor::eSrc1Alpha;
			case BlendFactor::ONE_MINUS_SRC1_ALPHA:
				return vk::BlendFactor::eOneMinusSrc1Alpha;

			default:
			{
				TUR_LOG_ERROR("Invalid Blend Factor. Using fallback.");
				return GetBlendFactor(fallback, fallback);
			} break;
		}
	}

	static constexpr vk::BlendOp GetBlendOperation(BlendOperation blendOperation)
	{
		switch (blendOperation)
		{
			case BlendOperation::ADD:
				return vk::BlendOp::eAdd;

			case BlendOperation::SUBTRACT: 
				return vk::BlendOp::eSubtract;

			case BlendOperation::REVERSE_SUBTRACT:
				return vk::BlendOp::eReverseSubtract;

			case BlendOperation::MIN:
				return vk::BlendOp::eMin;

			case BlendOperation::MAX:
				return vk::BlendOp::eMax;

			case BlendOperation::ZERO_EXT:
				return vk::BlendOp::eZeroEXT;

			case BlendOperation::SRC_EXT:
				return vk::BlendOp::eSrcEXT;

			case BlendOperation::DST_EXT:
				return vk::BlendOp::eDstEXT;

			case BlendOperation::SRC_OVER_EXT:
				return vk::BlendOp::eSrcOverEXT;

			case BlendOperation::DST_OVER_EXT:
				return vk::BlendOp::eDstOverEXT;

			case BlendOperation::SRC_IN_EXT:
				return vk::BlendOp::eSrcInEXT;

			case BlendOperation::DST_IN_EXT:
				return vk::BlendOp::eDstInEXT;

			case BlendOperation::SRC_OUT_EXT:
				return vk::BlendOp::eSrcOutEXT;

			case BlendOperation::DST_OUT_EXT:
				return vk::BlendOp::eDstOutEXT;

			case BlendOperation::SRC_ATOP_EXT:
				return vk::BlendOp::eSrcAtopEXT;

			case BlendOperation::DST_ATOP_EXT:
				return vk::BlendOp::eDstAtopEXT;

			case BlendOperation::XOR_EXT:
				return vk::BlendOp::eXorEXT;

			case BlendOperation::MULTIPLY_EXT:
				return vk::BlendOp::eMultiplyEXT;

			case BlendOperation::SCREEN_EXT:
				return vk::BlendOp::eScreenEXT;

			case BlendOperation::OVERLAY_EXT:
				return vk::BlendOp::eOverlayEXT;

			case BlendOperation::DARKEN_EXT:
				return vk::BlendOp::eDarkenEXT;

			case BlendOperation::LIGHTEN_EXT:
				return vk::BlendOp::eLightenEXT;

			case BlendOperation::COLORDODGE_EXT:
				return vk::BlendOp::eColordodgeEXT;

			case BlendOperation::COLORBURN_EXT:
				return vk::BlendOp::eColorburnEXT;

			case BlendOperation::HARDLIGHT_EXT:
				return vk::BlendOp::eHardlightEXT;

			case BlendOperation::SOFTLIGHT_EXT:
				return vk::BlendOp::eSoftlightEXT;

			case BlendOperation::DIFFERENCE_EXT:
				return vk::BlendOp::eDifferenceEXT;

			case BlendOperation::EXCLUSION_EXT:
				return vk::BlendOp::eExclusionEXT;

			case BlendOperation::INVERT_EXT:
				return vk::BlendOp::eInvertEXT;

			case BlendOperation::INVERT_RGB_EXT:
				return vk::BlendOp::eInvertRgbEXT;

			case BlendOperation::LINEARDODGE_EXT:
				return vk::BlendOp::eLineardodgeEXT;

			case BlendOperation::LINEARBURN_EXT:
				return vk::BlendOp::eLinearburnEXT;

			case BlendOperation::VIVIDLIGHT_EXT:
				return vk::BlendOp::eVividlightEXT;

			case BlendOperation::LINEARLIGHT_EXT:
				return vk::BlendOp::eLinearlightEXT;

			case BlendOperation::PINLIGHT_EXT:
				return vk::BlendOp::ePinlightEXT;

			case BlendOperation::HARDMIX_EXT:
				return vk::BlendOp::eHardmixEXT;

			case BlendOperation::HSL_HUE_EXT:
				return vk::BlendOp::eHslHueEXT;

			case BlendOperation::HSL_SATURATION_EXT:
				return vk::BlendOp::eHslSaturationEXT;

			case BlendOperation::HSL_COLOR_EXT:
				return vk::BlendOp::eHslColorEXT;

			case BlendOperation::HSL_LUMINOSITY_EXT:
				return vk::BlendOp::eHslLuminosityEXT;

			case BlendOperation::PLUS_EXT:
				return vk::BlendOp::ePlusEXT;

			case BlendOperation::PLUS_CLAMPED_EXT:
				return vk::BlendOp::ePlusClampedEXT;

			case BlendOperation::PLUS_CLAMPED_ALPHA_EXT:
				return vk::BlendOp::ePlusClampedAlphaEXT;

			case BlendOperation::PLUS_DARKER_EXT:
				return vk::BlendOp::ePlusDarkerEXT;

			case BlendOperation::MINUS_EXT:
				return vk::BlendOp::eMinusEXT;

			case BlendOperation::MINUS_CLAMPED_EXT:
				return vk::BlendOp::eMinusClampedEXT;

			case BlendOperation::CONTRAST_EXT:
				return vk::BlendOp::eContrastEXT;

			case BlendOperation::INVERT_OVG_EXT:
				return vk::BlendOp::eInvertOvgEXT;

			case BlendOperation::RED_EXT:
				return vk::BlendOp::eRedEXT;

			case BlendOperation::GREEN_EXT:
				return vk::BlendOp::eGreenEXT;

			case BlendOperation::BLUE_EXT:
				return vk::BlendOp::eBlueEXT;

			default:
			{
				TUR_LOG_ERROR("Invalid blend operation. Using default: ADD");
				return vk::BlendOp::eAdd;
			}
		}
	}

	static constexpr vk::LogicOp GetLogicOp(LogicOperation logicOp)
	{
		switch (logicOp)
		{
			case LogicOperation::CLEAR:
				return vk::LogicOp::eClear;

			case LogicOperation::AND:
				return vk::LogicOp::eAnd;

			case LogicOperation::AND_REVERSE:
				return vk::LogicOp::eAndReverse;

			case LogicOperation::COPY:
				return vk::LogicOp::eCopy;

			case LogicOperation::AND_INVERTED:
				return vk::LogicOp::eAndInverted;

			case LogicOperation::NO_OP:
				return vk::LogicOp::eNoOp;

			case LogicOperation::XOR:
				return vk::LogicOp::eXor;

			case LogicOperation::OR:
				return vk::LogicOp::eOr;

			case LogicOperation::NOR:
				return vk::LogicOp::eNor;

			case LogicOperation::EQUIVALENT:
				return vk::LogicOp::eEquivalent;

			case LogicOperation::INVERT:
				return vk::LogicOp::eInvert;

			case LogicOperation::OR_REVERSE:
				return vk::LogicOp::eOrReverse;

			case LogicOperation::COPY_INVERTED:
				return vk::LogicOp::eCopyInverted;

			case LogicOperation::OR_INVERTED:
				return vk::LogicOp::eOrInverted;

			case LogicOperation::NAND:
				return vk::LogicOp::eNand;

			case LogicOperation::SET:
				return vk::LogicOp::eSet;

			default:
			{
				TUR_LOG_ERROR("Invalid Color Blending Operation. Using default: COPY");
				return vk::LogicOp::eCopy;
			} break;
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