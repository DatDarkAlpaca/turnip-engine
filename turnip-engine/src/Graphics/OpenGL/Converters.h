#pragma once
#include <glad/glad.h>

#include "Common.h"
#include "Rendering/Type/Types.h"

#include "Rendering/Resource/Buffer.h"

namespace tur::gl
{
	inline constexpr uint32_t GetPolygonMode(PolygonMode polygonMode)
	{
		switch (polygonMode)
		{
		case PolygonMode::POINT:
			return GL_POINT;

		case PolygonMode::LINE:
			return GL_LINE;

		case PolygonMode::FILL:
			return GL_FILL;

		default:
		{
			TUR_LOG_ERROR("Invalid Polygon Mode. Using default: FILL");
			return GL_FILL;
		} break;
		}

		return 0;

	}

	inline constexpr uint32_t GetCullMode(CullMode cullMode)
	{
		switch (cullMode)
		{
		case CullMode::FRONT:
			return GL_FRONT;

		case CullMode::BACK:
			return GL_BACK;

		case CullMode::FRONT_AND_BACK:
			return GL_FRONT_AND_BACK;

		default:
		{
			TUR_LOG_ERROR("Invalid Polygon Mode. Using default: FRONT");
			return GL_BACK;
		}
		}

		return 0;

	}

	inline constexpr uint32_t GetFrontFace(FrontFace frontFace)
	{
		switch (frontFace)
		{
		case FrontFace::COUNTER_CLOCKWISE:
			return GL_CCW;

		case FrontFace::CLOCKWISE:
			return GL_CW;

		default:
		{
			TUR_LOG_ERROR("Invalid Front Face. Using default: CLOCKWISE");
			return GL_CW;
		} break;
		}

		return 0;

	}

	inline constexpr uint32_t GetBlendFactor(BlendFactor blendFactor, BlendFactor fallback = BlendFactor::ONE)
	{
		switch (blendFactor)
		{
		case BlendFactor::ZERO:
			return GL_ZERO;

		case BlendFactor::ONE:
			return GL_ONE;

		case BlendFactor::SRC_COLOR:
			return GL_SRC_COLOR;

		case BlendFactor::ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;

		case BlendFactor::DST_COLOR:
			return GL_DST_COLOR;

		case BlendFactor::ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;

		case BlendFactor::SRC_ALPHA:
			return GL_SRC_ALPHA;

		case BlendFactor::ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;

		case BlendFactor::DST_ALPHA:
			return GL_DST_ALPHA;

		case BlendFactor::ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;

		case BlendFactor::CONSTANT_COLOR:
			return GL_CONSTANT_COLOR;

		case BlendFactor::ONE_MINUS_CONSTANT_COLOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;

		case BlendFactor::CONSTANT_ALPHA:
			return GL_CONSTANT_ALPHA;

		case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			return GL_ONE_MINUS_CONSTANT_ALPHA;

		case BlendFactor::SRC_ALPHA_SATURATE:
		{
			TUR_LOG_WARN("Unsupported blend factor: SRC_ALPHA_SATURATE. Using fallback");
			return GetBlendFactor(fallback);
		} break;

		case BlendFactor::SRC1_COLOR:
		{
			TUR_LOG_WARN("Unsupported blend factor: SRC1_COLOR. Using fallback");
			return GetBlendFactor(fallback);
		} break;

		case BlendFactor::ONE_MINUS_SRC1_COLOR:
		{
			TUR_LOG_WARN("Unsupported blend factor: ONE_MINUS_SRC1_COLOR. Using fallback");
			return GetBlendFactor(fallback);
		} break;

		case BlendFactor::SRC1_ALPHA:
		{
			TUR_LOG_WARN("Unsupported blend factor: SRC1_ALPHA. Using fallback");
			return GetBlendFactor(fallback);
		} break;

		case BlendFactor::ONE_MINUS_SRC1_ALPHA:
		{
			TUR_LOG_WARN("Unsupported blend factor: ONE_MINUS_SRC1_ALPHA. Using fallback");
			return GetBlendFactor(fallback);
		} break;

		default:
		{
			TUR_LOG_ERROR("Invalid Blend Factor. Using fallback.");
			return GetBlendFactor(fallback);
		} break;
		}
	}

	static constexpr uint32_t GetBlendOperation(BlendOperation blendOperation)
	{
		switch (blendOperation)
		{
		case BlendOperation::ADD:
			return GL_FUNC_ADD;

		case BlendOperation::SUBTRACT:
			return GL_FUNC_SUBTRACT;

		case BlendOperation::REVERSE_SUBTRACT:
			return GL_FUNC_REVERSE_SUBTRACT;

		case BlendOperation::MIN:
			return GL_MIN;

		case BlendOperation::MAX:
			return GL_MAX;

		default:
		{
			TUR_LOG_ERROR("Invalid or unsupported blending operation. Using default: ADD");
			return GL_FUNC_ADD;
		} break;
		}
	}

	static constexpr uint32_t GetLogicOp(LogicOperation logicOp)
	{
		switch (logicOp)
		{
		case LogicOperation::CLEAR:
			return GL_CLEAR;

		case LogicOperation::AND:
			return GL_AND;

		case LogicOperation::AND_REVERSE:
			return GL_AND_REVERSE;

		case LogicOperation::COPY:
			return GL_COPY;

		case LogicOperation::AND_INVERTED:
			return GL_AND_INVERTED;

		case LogicOperation::NO_OP:
			return GL_NOOP;

		case LogicOperation::XOR:
			return GL_XOR;

		case LogicOperation::OR:
			return GL_OR;

		case LogicOperation::NOR:
			return GL_NOR;

		case LogicOperation::EQUIVALENT:
			return GL_EQUIV;

		case LogicOperation::INVERT:
			return GL_INVERT;

		case LogicOperation::OR_REVERSE:
			return GL_OR_REVERSE;

		case LogicOperation::COPY_INVERTED:
			return GL_COPY_INVERTED;

		case LogicOperation::OR_INVERTED:
			return GL_OR_INVERTED;

		case LogicOperation::NAND:
			return GL_NAND;

		case LogicOperation::SET:
			return GL_SET;

		default:
		{
			TUR_LOG_ERROR("Invalid Color Blending Operation. Using default: COPY");
			return GL_COPY;
		} break;
		}
	}
}

namespace tur::gl
{
	// TUR_LOG_WARN("R32_G32_SINT is not defined in OpenGL. Using GL_RG32UI");
	// return GL_RG32UI;

#define UNDEFINED_FORMAT(FormatName, Default) TUR_LOG_WARN("{} is not defined in OpenGL. Using {}", ##FormatName, ##Default); [[fallthrough]];

	struct FormatInfo
	{
		uint32_t formatID;
		uint32_t componentSize;
		uint32_t componentAmount;
	};

	inline constexpr FormatInfo GetFormat(Format format)
	{
		switch (format)
		{
			// Misc
			{
		case Format::R4G4_UNORM_PACK8:
			UNDEFINED_FORMAT("R4G4_UNORM_PACK8", "GL_NONE");

		case Format::R4G4B4A4_UNORM_PACK16:
			UNDEFINED_FORMAT("R4G4B4A4_UNORM_PACK16", "GL_NONE");

		case Format::B4G4R4A4_UNORM_PACK16:
			UNDEFINED_FORMAT("B4G4R4A4_UNORM_PACK16", "GL_NONE");

		case Format::R5G6B5_UNORM_PACK16:
			UNDEFINED_FORMAT("R5G6B5_UNORM_PACK16", "GL_NONE");

		case Format::B5G6R5_UNORM_PACK16:
			UNDEFINED_FORMAT("B5G6R5_UNORM_PACK16", "GL_NONE");

		case Format::R5G5B5A1_UNORM_PACK16:
			UNDEFINED_FORMAT("R5G5B5A1_UNORM_PACK16", "GL_NONE");

		case Format::B5G5R5A1_UNORM_PACK16:
			UNDEFINED_FORMAT("B5G5R5A1_UNORM_PACK16", "GL_NONE");

		case Format::A1R5G5B5_UNORM_PACK16:
			return { GL_NONE, sizeof(int), 1 };
			}

			// R (8):
			{
		case Format::R8:
			return { GL_R8, sizeof(uint8_t), 1 };

		case Format::R8_UNORM:
			UNDEFINED_FORMAT("R8_UNORM", "GL_R8_SNORM");

		case Format::R8_SNORM:
			return { GL_R8_SNORM, sizeof(uint8_t), 1 };

		case Format::R8_USCALED:
			UNDEFINED_FORMAT("R8_USCALED", "GL_R8UI");

		case Format::R8_SSCALED:
			UNDEFINED_FORMAT("R8_SSCALED", "GL_R8UI");

		case Format::R8_UINT:
			return { GL_R8UI, sizeof(uint8_t), 1 };

		case Format::R8_SINT:
			return { GL_R8_SNORM, sizeof(uint8_t), 1 };

		case Format::R8_SRGB:
			return { GL_SRGB8, sizeof(uint8_t), 1 };
			}

			// RG (8):
			{
		case Format::R8G8:
			return { GL_RG8, sizeof(uint8_t), 2 };

		case Format::R8G8_UNORM:
			UNDEFINED_FORMAT("R8G8_UNORM", "GL_RG8_SNORM");

		case Format::R8G8_SNORM:
			return { GL_RG8_SNORM, sizeof(uint8_t), 2 };

		case Format::R8G8_USCALED:
			UNDEFINED_FORMAT("R8G8_USCALED", "GL_RG8UI");

		case Format::R8G8_SSCALED:
			UNDEFINED_FORMAT("R8G8_SSCALED", "GL_RG8UI");

		case Format::R8G8_UINT:
			return { GL_RG8UI, sizeof(uint8_t), 2 };

		case Format::R8G8_SINT:
			return { GL_RG8_SNORM, sizeof(uint8_t), 2 };

		case Format::R8G8_SRGB:
			return { GL_SRGB8, sizeof(uint8_t), 2 };
			}

			// RGB (8):
			{
		case Format::R8G8B8:
			return { GL_RGB8, sizeof(uint8_t), 3 };

		case Format::R8G8B8_UNORM:
			UNDEFINED_FORMAT("R8G8B8_UNORM", "GL_RGB8_SNORM");

		case Format::R8G8B8_SNORM:
			return { GL_RGB8_SNORM, sizeof(uint8_t), 3 };

		case Format::R8G8B8_USCALED:
			UNDEFINED_FORMAT("R8G8B8_USCALED", "GL_RGB8UI");

		case Format::R8G8B8_SSCALED:
			UNDEFINED_FORMAT("R8G8B8_SSCALED", "GL_RGB8UI");

		case Format::R8G8B8_UINT:
			return { GL_RGB8UI, sizeof(uint8_t), 3 };

		case Format::R8G8B8_SINT:
			return { GL_RGB8_SNORM, sizeof(uint8_t), 3 };

		case Format::R8G8B8_SRGB:
			return { GL_SRGB8, sizeof(uint8_t), 3 };
			}

			// BGR (8):
			{
		case Format::B8G8R8:
			return { GL_BGR, sizeof(uint8_t), 3 };

		case Format::B8G8R8_UNORM:
			UNDEFINED_FORMAT("B8G8R8_UNORM", "GL_BGR");

		case Format::B8G8R8_SNORM:
			UNDEFINED_FORMAT("B8G8R8_SNORM", "GL_BGR");

		case Format::B8G8R8_USCALED:
			UNDEFINED_FORMAT("B8G8R8_USCALED", "GL_BGR");

		case Format::B8G8R8_SSCALED:
			UNDEFINED_FORMAT("B8G8R8_SSCALED", "GL_BGR");

		case Format::B8G8R8_UINT:
			UNDEFINED_FORMAT("B8G8R8_UINT", "GL_BGR");

		case Format::B8G8R8_SINT:
			UNDEFINED_FORMAT("B8G8R8_SINT", "GL_BGR");

		case Format::B8G8R8_SRGB:
		{
			UNDEFINED_FORMAT("B8G8R8_SINT", "GL_BGR");
			return { GL_BGR, sizeof(uint8_t), 3 };
		}
			}

			// RGBA (8):
			{
		case Format::R8G8B8A8:
			return { GL_RGBA8, sizeof(uint8_t), 4 };

		case Format::R8G8B8A8_UNORM:
			return { GL_RGBA8, sizeof(uint8_t), 4 };

		case Format::R8G8B8A8_SNORM:
			return { GL_RGBA8_SNORM, sizeof(uint8_t), 4 };

		case Format::R8G8B8A8_USCALED:
			UNDEFINED_FORMAT("R8G8B8A8_USCALED", "GL_RGBA8UI");

		case Format::R8G8B8A8_SSCALED:
			UNDEFINED_FORMAT("R8G8B8A8_SSCALED", "GL_RGBA8UI");

		case Format::R8G8B8A8_UINT:
			return { GL_RGBA8UI, sizeof(uint8_t), 4 };

		case Format::R8G8B8A8_SINT:
			UNDEFINED_FORMAT("R8G8B8A8_SINT", "GL_BGRA");

		case Format::R8G8B8A8_SRGB:
			UNDEFINED_FORMAT("R8G8B8A8_SRGB", "GL_BGRA");
			return { GL_BGRA, sizeof(uint8_t), 4 };
			}

			// BGRA (8):
			{
		case Format::B8G8R8A8:
			UNDEFINED_FORMAT("B8G8R8A8", "GL_BGRA");

		case Format::B8G8R8A8_UNORM:
			UNDEFINED_FORMAT("B8G8R8A8_UNORM", "GL_BGRA");

		case Format::B8G8R8A8_SNORM:
			UNDEFINED_FORMAT("B8G8R8A8_SNORM", "GL_BGRA");

		case Format::B8G8R8A8_USCALED:
			UNDEFINED_FORMAT("B8G8R8A8_USCALED", "GL_BGRA");

		case Format::B8G8R8A8_SSCALED:
			UNDEFINED_FORMAT("B8G8R8A8_SSCALED", "GL_BGRA");

		case Format::B8G8R8A8_UINT:
			UNDEFINED_FORMAT("B8G8R8A8_UINT", "GL_BGRA");

		case Format::B8G8R8A8_SINT:
			UNDEFINED_FORMAT("B8G8R8A8_SINT", "GL_BGRA");

		case Format::B8G8R8A8_SRGB:
		{
			UNDEFINED_FORMAT("B8G8R8A8_SRGB", "GL_BGRA");
			return { GL_BGRA, sizeof(uint8_t), 4 };
		}
			}

			// ABGR (8):
			{
		case Format::A8B8G8R8_UNORM_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_UNORM_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_SNORM_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_SNORM_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_USCALED_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_USCALED_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_SSCALED_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_SSCALED_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_UINT_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_UINT_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_SINT_PACK32:
			UNDEFINED_FORMAT("A8B8G8R8_SINT_PACK32", "GL_BGRA");

		case Format::A8B8G8R8_SRGB_PACK32:
		{
			UNDEFINED_FORMAT("A8B8G8R8_SRGB_PACK32", "GL_BGRA");
			return { GL_BGRA, sizeof(uint8_t), 4 };
		}
			}

			// A2 RGB (10):
			{
		case Format::A2R10G10B10_UNORM_PACK32:
			UNDEFINED_FORMAT("A2R10G10B10_UNORM_PACK32", "GL_BGRA");

		case Format::A2R10G10B10_SNORM_PACK32:
			UNDEFINED_FORMAT("A2R10G10B10_SNORM_PACK32", "GL_BGRA");

		case Format::A2R10G10B10_USCALED_PACK32:
			UNDEFINED_FORMAT("A2R10G10B10_USCALED_PACK32", "GL_BGRA");

		case Format::A2R10G10B10_SSCALED_PACK32:
			UNDEFINED_FORMAT("A2R10G10B10_SSCALED_PACK32", "GL_BGRA");

		case Format::A2R10G10B10_UINT_PACK32:
			UNDEFINED_FORMAT("A2R10G10B10_UINT_PACK32", "GL_BGRA");

		case Format::A2R10G10B10_SINT_PACK32:
		{
			UNDEFINED_FORMAT("A2R10G10B10_SINT_PACK32", "GL_BGRA");
			return { GL_BGRA, sizeof(uint8_t), 4 };
		}
			}

			// A2 BGR (10):
			{
		case Format::A2B10G10R10_UNORM_PACK32:
			UNDEFINED_FORMAT("A2B10G10R10_UNORM_PACK32", "GL_BGRA");

		case Format::A2B10G10R10_SNORM_PACK32:
			UNDEFINED_FORMAT("A2B10G10R10_SNORM_PACK32", "GL_BGRA");

		case Format::A2B10G10R10_USCALED_PACK32:
			UNDEFINED_FORMAT("A2B10G10R10_USCALED_PACK32", "GL_BGRA");

		case Format::A2B10G10R10_SSCALED_PACK32:
			UNDEFINED_FORMAT("A2B10G10R10_SSCALED_PACK32", "GL_BGRA");

		case Format::A2B10G10R10_UINT_PACK32:
			UNDEFINED_FORMAT("A2B10G10R10_UINT_PACK32", "GL_BGRA");

		case Format::A2B10G10R10_SINT_PACK32:
		{
			UNDEFINED_FORMAT("A2B10G10R10_SINT_PACK32", "GL_BGRA");
			return { GL_BGRA, sizeof(uint8_t), 4 };
		}
			}

			// R (16):
			{
		case Format::R16:
			return { GL_R16, sizeof(uint16_t), 1 };

		case Format::R16_UNORM:
			return { GL_R16, sizeof(uint16_t), 1 };

		case Format::R16_SNORM:
			return { GL_R16_SNORM, sizeof(uint16_t), 1 };

		case Format::R16_USCALED:
			UNDEFINED_FORMAT("R16_USCALED", "GL_R16UI");

		case Format::R16_SSCALED:
			UNDEFINED_FORMAT("R16_SSCALED", "GL_R16UI");

		case Format::R16_UINT:
			return { GL_R16UI, sizeof(uint16_t), 1 };

		case Format::R16_SINT:
			return { GL_R16_SNORM, sizeof(uint16_t), 1 };

		case Format::R16_SFLOAT:
			return { GL_R16F, sizeof(uint16_t), 1 };
			}

			// RG (16):
			{
		case Format::R16G16:
			return { GL_RG16, sizeof(uint16_t), 2 };

		case Format::R16G16_UNORM:
			return { GL_RG16, sizeof(uint16_t), 2 };

		case Format::R16G16_SNORM:
			return { GL_RG16_SNORM, sizeof(uint16_t), 2 };

		case Format::R16G16_USCALED:
			UNDEFINED_FORMAT("R16G16_USCALED", "GL_RG16UI");

		case Format::R16G16_SSCALED:
			UNDEFINED_FORMAT("R16G16_SSCALED", "GL_RG16UI");

		case Format::R16G16_UINT:
			return { GL_RG16UI, sizeof(uint16_t), 2 };

		case Format::R16G16_SINT:
			return { GL_RG16_SNORM, sizeof(uint16_t), 2 };

		case Format::R16G16_SFLOAT:
			return { GL_RG16F, sizeof(uint16_t), 2 };
			}

			// RGB (16):
			{
		case Format::R16G16B16:
			return { GL_RGB16, sizeof(uint16_t), 3 };

		case Format::R16G16B16_UNORM:
			return { GL_RGB16, sizeof(uint16_t), 3 };

		case Format::R16G16B16_SNORM:
			return { GL_RGB16_SNORM, sizeof(uint16_t), 3 };

		case Format::R16G16B16_USCALED:
			UNDEFINED_FORMAT("R16G16B16_USCALED", "GL_RGB16UI");

		case Format::R16G16B16_SSCALED:
			UNDEFINED_FORMAT("R16G16B16_SSCALED", "GL_RGB16UI");

		case Format::R16G16B16_UINT:
			return { GL_RGB16UI, sizeof(uint16_t), 3 };

		case Format::R16G16B16_SINT:
			return { GL_RGB16_SNORM, sizeof(uint16_t), 3 };

		case Format::R16G16B16_SFLOAT:
			return { GL_RGB16F, sizeof(uint16_t), 3 };
			}

			// RGBA (16):
			{
		case Format::R16G16B16A16:
			return { GL_RGBA16, sizeof(uint16_t), 4 };

		case Format::R16G16B16A16_UNORM:
			return { GL_RGBA16, sizeof(uint16_t), 4 };

		case Format::R16G16B16A16_SNORM:
			return { GL_RGBA16_SNORM, sizeof(uint16_t), 4 };

		case Format::R16G16B16A16_USCALED:
			UNDEFINED_FORMAT("R16G16B16A16_USCALED", "GL_RGBA16UI");

		case Format::R16G16B16A16_SSCALED:
			UNDEFINED_FORMAT("R16G16B16A16_SSCALED", "GL_RGBA16UI");

		case Format::R16G16B16A16_UINT:
			return { GL_RGBA16UI, sizeof(uint16_t), 4 };

		case Format::R16G16B16A16_SINT:
			return { GL_RGBA16_SNORM, sizeof(uint16_t), 4 };

		case Format::R16G16B16A16_SFLOAT:
			return { GL_RGBA16F, sizeof(uint16_t), 4 };
			}

			// R (32):
			{
		case Format::R32:
			return { GL_R32I, sizeof(uint32_t), 1 };

		case Format::R32_UINT:
			return { GL_R32UI, sizeof(uint32_t), 1 };

		case Format::R32_SINT:
		{
			UNDEFINED_FORMAT("R32_SINT", "GL_R32UI");
			return { GL_R32UI, sizeof(uint32_t), 1 };
		}

		case Format::R32_SFLOAT:
			return { GL_R32F, sizeof(uint32_t), 1 };
			}

			// RG (32):
			{
		case Format::R32G32:
			return { GL_RG32I, sizeof(uint32_t), 2 };

		case Format::R32G32_UINT:
			return { GL_RG32UI, sizeof(uint32_t), 2 };

		case Format::R32G32_SINT:
		{
			UNDEFINED_FORMAT("R32G32_SINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint32_t), 2 };
		}

		case Format::R32G32_SFLOAT:
			return { GL_RG32F, sizeof(uint32_t), 2 };
			}

			// RGB (32):
			{
		case Format::R32G32B32:
			return { GL_RGB32I, sizeof(uint32_t), 3 };

		case Format::R32G32B32_UINT:
			return { GL_RGB32I, sizeof(uint32_t), 3 };

		case Format::R32G32B32_SINT:
		{
			UNDEFINED_FORMAT("R32G32B32_SINT", "GL_RGB32I");
			return { GL_RGB32I, sizeof(uint32_t), 3 };
		}

		case Format::R32G32B32_SFLOAT:
			return { GL_RGB32F, sizeof(uint32_t), 3 };
			}

			// RGB (32):
			{
		case Format::R32G32B32A32:
			return { GL_RGBA32UI, sizeof(uint32_t), 3 };

		case Format::R32G32B32A32_UINT:
			return { GL_RGBA32UI, sizeof(uint32_t), 3 };

		case Format::R32G32B32A32_SINT:
			return { GL_RGBA32I, sizeof(uint32_t), 3 };

		case Format::R32G32B32A32_SFLOAT:
			return { GL_RGBA32F, sizeof(uint32_t), 3 };
			}

			// R (64):
			{
		case Format::R64:
		{
			UNDEFINED_FORMAT("R64", "GL_R32UI");
			return { GL_R32UI, sizeof(uint64_t), 1 };
		}

		case Format::R64_UINT:
		{
			UNDEFINED_FORMAT("R64_UINT", "GL_R32UI");
			return { GL_R32UI, sizeof(uint64_t), 1 };
		}

		case Format::R64_SINT:
		{
			UNDEFINED_FORMAT("R64_SINT", "GL_R32UI");
			return { GL_R32UI, sizeof(uint64_t), 1 };
		}

		case Format::R64_SFLOAT:
		{
			UNDEFINED_FORMAT("R64_SFLOAT", "GL_R32F");
			return { GL_R32F, sizeof(uint64_t), 1 };
		}
			}

			// RG (64):
			{
		case Format::R64G64:
		{
			UNDEFINED_FORMAT("R64G64", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 2 };
		}

		case Format::R64G64_UINT:
		{
			UNDEFINED_FORMAT("R64G64_UINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 2 };
		}

		case Format::R64G64_SINT:
		{
			UNDEFINED_FORMAT("R64G64_SINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 2 };
		}

		case Format::R64G64_SFLOAT:
		{
			UNDEFINED_FORMAT("R64G64_SFLOAT", "GL_RG32F");
			return { GL_RG32F, sizeof(uint64_t), 2 };
		}
			}

			// RGB (64):
			{
		case Format::R64G64B64:
		{
			UNDEFINED_FORMAT("R64G64B64", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 3 };
		}

		case Format::R64G64B64_UINT:
		{
			UNDEFINED_FORMAT("R64G64B64_UINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 3 };
		}

		case Format::R64G64B64_SINT:
		{
			UNDEFINED_FORMAT("R64G64B64_SINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 3 };
		}

		case Format::R64G64B64_SFLOAT:
		{
			UNDEFINED_FORMAT("R64G64B64_SFLOAT", "GL_RG32F");
			return { GL_RG32F, sizeof(uint64_t), 3 };
		}
			}

			// RGBA (64):
			{
		case Format::R64G64B64A64:
		{
			UNDEFINED_FORMAT("R64G64B64A64", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 4 };
		}

		case Format::R64G64B64A64_UINT:
		{
			UNDEFINED_FORMAT("R64G64B64A64_UINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 4 };
		}

		case Format::R64G64B64A64_SINT:
		{
			UNDEFINED_FORMAT("R64G64B64A64_SINT", "GL_RG32UI");
			return { GL_RG32UI, sizeof(uint64_t), 4 };
		}

		case Format::R64G64B64A64_SFLOAT:
		{
			UNDEFINED_FORMAT("R64G64B64A64_SFLOAT", "GL_RG32F");
			return { GL_RG32F, sizeof(uint64_t), 4 };
		}
			}

			// B10 GR (11) P32
		case Format::B10G11R11_UFLOAT_PACK32:
		{
			UNDEFINED_FORMAT("B10G11R11_UFLOAT_PACK32", "GL_BGR");
			return { GL_BGR, sizeof(uint32_t), 3 };
		}

		default:
		{
			TUR_LOG_ERROR("Invalid Format. Using default: R32_UINT");
			return { GL_R32UI, sizeof(uint32_t), 1 };
		} break;
		}
	}

	inline constexpr uint32_t GetAttributeFormat(Format format)
	{
		switch (format)
		{
			case Format::R8:
			case Format::R8G8:
			case Format::R8G8B8:
			case Format::B8G8R8:
			case Format::R8G8B8A8:
			case Format::B8G8R8A8:
			case Format::R16:
			case Format::R16G16:
			case Format::R16G16B16:
			case Format::R16G16B16A16:
			case Format::R32:
			case Format::R32G32:
			case Format::R32G32B32:
			case Format::R32G32B32A32:
			case Format::R64:
			case Format::R64G64:
			case Format::R64G64B64:
			case Format::R64G64B64A64:
				return GL_INT;

			case Format::R8_UINT:
			case Format::R8G8_UINT:
			case Format::R8G8B8_UINT:
			case Format::R8G8B8A8_UINT:
			case Format::B8G8R8A8_UINT:
			case Format::A8B8G8R8_UINT_PACK32:
			case Format::A2R10G10B10_UINT_PACK32:
			case Format::R16_UINT:
			case Format::R16G16_UINT:
			case Format::R16G16B16_UINT:
			case Format::R16G16B16A16_UINT:
			case Format::R32_UINT:
			case Format::R32G32_UINT:
			case Format::R32G32B32_UINT:
			case Format::R32G32B32A32_UINT:
			case Format::R64_UINT:
			case Format::R64G64_UINT:
			case Format::R64G64B64_UINT:
			case Format::R64G64B64A64_UINT:
				return GL_UNSIGNED_INT;


			case Format::R16_SFLOAT:
			case Format::R16G16_SFLOAT:
			case Format::R16G16B16_SFLOAT:
			case Format::R16G16B16A16_SFLOAT:
			case Format::R32_SFLOAT:
			case Format::R32G32_SFLOAT:
			case Format::R32G32B32_SFLOAT:
			case Format::R32G32B32A32_SFLOAT:
			case Format::R64_SFLOAT:
			case Format::R64G64_SFLOAT:
			case Format::R64G64B64_SFLOAT:
			case Format::R64G64B64A64_SFLOAT:
			case Format::B10G11R11_UFLOAT_PACK32:
				return GL_FLOAT;
		}
	}


	inline constexpr uint32_t GetPrimitiveTopology(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::POINTS:
				return GL_POINTS;

			case PrimitiveTopology::LINES: 
				return GL_LINES;

			case PrimitiveTopology::LINE_STRIPS: 
				return GL_LINE_STRIP;

			case PrimitiveTopology::TRIANGLES: 
				return GL_TRIANGLES;

			case PrimitiveTopology::TRIANGLE_STRIPS: 
				return GL_TRIANGLE_STRIP;

			case PrimitiveTopology::TRIANGLE_FANS: 
				return GL_TRIANGLE_FAN;

			case PrimitiveTopology::LINES_WITH_ADJACENCY:
			{
				TUR_LOG_WARN("LINES_WITH_ADJACENCY is not defined in OpenGL. Using LINES");
				return GL_LINES;
			}

			case PrimitiveTopology::LINE_STRIPS_WITH_ADJACENCY: 
			{
				TUR_LOG_WARN("LINE_STRIPS_WITH_ADJACENCY is not defined in OpenGL. Using LINE_STRIP");
				return GL_LINE_STRIP;
			}

			case PrimitiveTopology::TRIANGLES_WITH_ADJACENCY:
			{
				TUR_LOG_WARN("TRIANGLES_WITH_ADJACENCY is not defined in OpenGL. Using TRIANGLES");
				return GL_TRIANGLES;
			}

			case PrimitiveTopology::TRIANGLE_STRIPS_WITH_ADJACENCY: 
			{
				TUR_LOG_WARN("TRIANGLE_STRIPS_WITH_ADJACENCY is not defined in OpenGL. Using TRIANGLE_STRIP");
				return GL_TRIANGLE_STRIP;
			}

			case PrimitiveTopology::PATCHES: 
				return GL_PATCHES;

			default:
			{
				TUR_LOG_ERROR("Invalid Primitive Topology. Using default: GL_POINTS");
				return GL_POINTS;
			} break;
		}
	}

	// Flags:
	inline constexpr uint32_t GetClearFlag(ClearFlags flags)
	{
		switch (flags)
		{
			case ClearFlags::COLOR:
				return GL_COLOR_BUFFER_BIT;

			case ClearFlags::DEPTH:
				return GL_DEPTH_BUFFER_BIT;

			case ClearFlags::STENCIL:
				return GL_STENCIL_BUFFER_BIT;

			case ClearFlags::COLOR_DEPTH:
				return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

			case ClearFlags::DEPTH_STENCIL:
				return GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

			case ClearFlags::ALL:
				return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

			default:
			{
				TUR_LOG_ERROR("Invalid Clear Flag. Using default: GL_COLOR_BUFFER_BIT");
				return GL_COLOR_BUFFER_BIT;
			} break;
		}
	}

	// Buffer:
	inline uint32_t GetBufferUsageFlags(const State<UsageFlag>& flags)
	{
		uint32_t results = 0;

		if (flags[UsageFlag::ARRAY_BUFFER])
			results |= GL_ARRAY_BUFFER;

		if (flags[UsageFlag::INDEX_BUFFER])
			results |= GL_ELEMENT_ARRAY_BUFFER;

		if (flags[UsageFlag::UNIFORM_BUFFER])
			results |= GL_UNIFORM_BUFFER;

		if (flags[UsageFlag::TRANSFER_DST])
			TUR_LOG_WARN("Usage of TRANSFER_DST is not supported by OpenGL. Ignoring value.");

		if (flags[UsageFlag::TRANSFER_SRC])
			TUR_LOG_WARN("Usage of TRANSFER_SRC is not supported by OpenGL. Ignoring value.");

		return results;
	}

	inline constexpr uint32_t GetBuferDataStorageType(DataStorage flag)
	{
		switch (flag)
		{
			case DataStorage::STREAM_DRAW:
				return GL_STREAM_DRAW;

			case DataStorage::STREAM_READ:
				return GL_STREAM_READ;

			case DataStorage::STREAM_COPY:
				return GL_STREAM_COPY;

			case DataStorage::STATIC_DRAW:
				return GL_STATIC_DRAW;

			case DataStorage::STATIC_READ:
				return GL_STATIC_READ;

			case DataStorage::STATIC_COPY:
				return GL_STATIC_COPY;

			case DataStorage::DYNAMIC_DRAW:
				return GL_DYNAMIC_DRAW;

			case DataStorage::DYNAMIC_READ:
				return GL_DYNAMIC_READ;

			case DataStorage::DYNAMIC_COPY:
				return GL_DYNAMIC_COPY;

			default:
				TUR_LOG_CRITICAL("Invalid buffer usage flag.");
				break;
		}

		return 0xFFFFFFFF;
	}
}