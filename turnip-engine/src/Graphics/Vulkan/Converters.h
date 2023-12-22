#pragma once
#include "Graphics/Types/PrimitiveTopology.h"
#include "Graphics/Types/Viewport.h"
#include "Graphics/Types/Scissor.h"
#include "Graphics/Types/PolygonMode.h"
#include "Graphics/Types/CullMode.h"
#include "Graphics/Types/FrontFace.h"
#include "Graphics/Types/ColorBlending.h"
#include "Graphics/Types/AttachmentOperation.h"
#include "Graphics/Types/ImageLayout.h"

namespace tur::vulkan
{
	inline constexpr vk::PrimitiveTopology GetTopology(PrimitiveTopology topology)
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

	inline constexpr vk::PolygonMode GetPolygonMode(PolygonMode polygonMode)
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

	inline constexpr vk::CullModeFlags GetCullMode(CullMode cullMode)
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

	inline constexpr vk::FrontFace GetFrontFace(FrontFace frontFace)
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

	inline constexpr vk::SampleCountFlagBits GetSampleCount(uint32_t samples)
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

	inline inline vk::ColorComponentFlags GetColorComponentFlags(ColorComponents components)
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

	inline constexpr vk::BlendFactor GetBlendFactor(BlendFactor blendFactor, BlendFactor fallback)
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

	inline constexpr vk::BlendOp GetBlendOperation(BlendOperation blendOperation)
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

	inline constexpr vk::LogicOp GetLogicOp(LogicOperation logicOp)
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

	inline constexpr vk::AttachmentLoadOp GetAttachmentLoadOp(AttachmentLoadOperation operation)
	{
		switch (operation)
		{
			case AttachmentLoadOperation::NONE:
				return vk::AttachmentLoadOp::eNoneEXT;

			case AttachmentLoadOperation::LOAD:
				return vk::AttachmentLoadOp::eLoad;

			case AttachmentLoadOperation::CLEAR:
				return vk::AttachmentLoadOp::eClear;

			case AttachmentLoadOperation::DONT_CARE:
				return vk::AttachmentLoadOp::eDontCare;

			default:
			{
				TUR_LOG_ERROR("Invalid Attachament Load Operation. Using default: DONT CARE");
				return vk::AttachmentLoadOp::eDontCare;
			} break;
		}
	}

	inline constexpr vk::AttachmentStoreOp GetAttachmentStoreOp(AttachmentStoreOperation operation)
	{
		switch (operation)
		{
			case AttachmentStoreOperation::NONE:
				return vk::AttachmentStoreOp::eNoneEXT;

			case AttachmentStoreOperation::STORE:
				return vk::AttachmentStoreOp::eStore;

			case AttachmentStoreOperation::DONT_CARE:
				return vk::AttachmentStoreOp::eDontCare;

			default:
			{
				TUR_LOG_ERROR("Invalid Attachament Store Operation. Using default: DONT CARE");
				return vk::AttachmentStoreOp::eDontCare;
			} break;
		}
	}

	inline constexpr vk::ImageLayout GetImageLayout(ImageLayout layout)
	{
		switch (layout)
		{
			case ImageLayout::UNDEFINED:
				return vk::ImageLayout::eUndefined;

			case ImageLayout::GENERAL:
				return vk::ImageLayout::eGeneral;

			case ImageLayout::COLOR_ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eColorAttachmentOptimal;

			case ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eStencilAttachmentOptimal;

			case ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eStencilReadOnlyOptimal;

			case ImageLayout::SHADER_READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eShaderReadOnlyOptimal;

			case ImageLayout::TRANSFER_SRC_OPTIMAL:
				return vk::ImageLayout::eTransferSrcOptimal;

			case ImageLayout::TRANSFER_DST_OPTIMAL:
				return vk::ImageLayout::eTransferDstOptimal;

			case ImageLayout::PREINITIALIZED:
				return vk::ImageLayout::ePreinitialized;

			case ImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;

			case ImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal;

			case ImageLayout::DEPTH_ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eDepthAttachmentOptimal;

			case ImageLayout::DEPTH_READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eDepthReadOnlyOptimal;

			case ImageLayout::STENCIL_ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eStencilAttachmentOptimal;

			case ImageLayout::STENCIL_READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eStencilReadOnlyOptimal;

			case ImageLayout::READ_ONLY_OPTIMAL:
				return vk::ImageLayout::eReadOnlyOptimal;

			case ImageLayout::ATTACHMENT_OPTIMAL:
				return vk::ImageLayout::eAttachmentOptimal;

			case ImageLayout::PRESENT_SRC:
				return vk::ImageLayout::ePresentSrcKHR;

			case ImageLayout::SHARED_PRESENT:
				return vk::ImageLayout::eSharedPresentKHR;

			default:
			{
				TUR_LOG_ERROR("Invalid Image Layout. Using default: PRESENT_SRC_KHR");
				return vk::ImageLayout::ePresentSrcKHR;
			} break;
		}
	}
}