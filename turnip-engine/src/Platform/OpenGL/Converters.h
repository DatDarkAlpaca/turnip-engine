#pragma once
#include <glad/glad.h>

#include "Common.h"
#include "Rendering/Type/Types.h"
#include "Rendering/Type/LayoutTypes.h"

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

	inline constexpr uint32_t GetFormat(DataFormat format)
	{
		switch (format)
		{
			case DataFormat::UNDEFINED:
				return -1;

			case DataFormat::R8_UNORM:
				return GL_R8;

			case DataFormat::R8_SNORM:
				return GL_R8_SNORM;

			case DataFormat::R8_UINT:
				return GL_R8UI;

			case DataFormat::R8_SINT:
			{
				TUR_LOG_WARN("R8_SINT is not defined in OpenGL. Using GL_R8UI");
				return GL_R8UI;
			}

			case DataFormat::R8_SRGB:
				return GL_SRGB8;

			case DataFormat::R8_G8_UNORM:
				return GL_RG8;

			case DataFormat::R8_G8_SNORM:
				return GL_RG8_SNORM;

			case DataFormat::R8_G8_UINT:
				return GL_RG8UI;

			case DataFormat::R8_G8_SINT:
			{
				TUR_LOG_WARN("R8_G8_SINT is not defined in OpenGL. Using GL_RG8UI");
				return GL_RG8UI;
			}

			case DataFormat::R8_G8_SRGB:
			{
				TUR_LOG_WARN("R8_G8_SRGB is not defined in OpenGL. Using GL_RG8UI");
				return GL_RG8UI;
			}

			case DataFormat::R8_G8_B8_UNORM:
				return GL_RGB8;

			case DataFormat::R8_G8_B8_SNORM:
				return GL_SRGB8;

			case DataFormat::R8_G8_B8_UINT:
				return GL_RGB8I;

			case DataFormat::R8_G8_B8_SINT:
			{
				TUR_LOG_WARN("R8_G8_B8_SINT is not defined in OpenGL. Using GL_RGB8UI");
				return GL_RGB8UI;
			}

			case DataFormat::R8_G8_B8_SRGB:
				return GL_RGB8_SNORM;

			case DataFormat::R8_G8_B8_A8_UNORM:
				return GL_RGBA8;

			case DataFormat::R8_G8_B8_A8_SNORM:
				return GL_RGBA8_SNORM;

			case DataFormat::R8_G8_B8_A8_UINT:
				return GL_RGBA8I;

			case DataFormat::R8_G8_B8_A8_SINT:
			{
				TUR_LOG_WARN("R8_G8_B8_A8_SINT is not defined in OpenGL. Using GL_RGBA8UI");
				return GL_RGBA8UI;
			}

			case DataFormat::R8_G8_B8_A8_SRGB:
				return GL_RGBA8_SNORM;

			case DataFormat::R16_UNORM:
				return GL_R16;

			case DataFormat::R16_SNORM:
				return GL_R16_SNORM;

			case DataFormat::R16_UINT:
				return GL_R16I;

			case DataFormat::R16_SINT:
			{
				TUR_LOG_WARN("R16_SINT is not defined in OpenGL. Using GL_R16UI");
				return GL_R16UI;
			}

			case DataFormat::R16_SFLOAT:
				return GL_R16F;

			case DataFormat::R16_G16_UNORM:
				return GL_RG16;

			case DataFormat::R16_G16_SNORM:
				return GL_RG16_SNORM;

			case DataFormat::R16_G16_UINT:
				return GL_RG16UI;

			case DataFormat::R16_G16_SINT:
			{
				TUR_LOG_WARN("R16_G16_SINT is not defined in OpenGL. Using GL_RG16UI");
				return GL_RG16UI;
			}

			case DataFormat::R16_G16_SFLOAT:
				return GL_RG16F;

			case DataFormat::R16_G16_B16_UNORM:
				return GL_RGB16;

			case DataFormat::R16_G16_B16_SNORM:
				return GL_RGB16_SNORM;

			case DataFormat::R16_G16_B16_UINT:
				return GL_RGB16UI;

			case DataFormat::R16_G16_B16_SINT:
			{
				TUR_LOG_WARN("R16_G16_B16_SINT is not defined in OpenGL. Using GL_RGB16UI");
				return GL_RGB16UI;
			}

			case DataFormat::R16_G16_B16_SFLOAT:
				return GL_RGB16F;

			case DataFormat::R16_G16_B16_A16UNORM:
				return GL_RGBA16;

			case DataFormat::R16_G16_B16_A16_SNORM:
				return GL_RGBA16_SNORM;

			case DataFormat::R16_G16_B16_A16_UINT:
				return GL_RGBA16UI;

			case DataFormat::R16_G16_B16_A16_SINT:
			{
				TUR_LOG_WARN("R16_G16_B16_A16_SINT is not defined in OpenGL. Using GL_RGBA16UI");
				return GL_RGBA16UI;
			}

			case DataFormat::R16_G16_B16_A16_SFLOAT:
				return GL_RGBA16F;

			case DataFormat::R32_UINT:
				return GL_R32UI;

			case DataFormat::R32_SINT:
			{
				TUR_LOG_WARN("R32_SINT is not defined in OpenGL. Using GL_R32UI");
				return GL_R32UI;
			}

			case DataFormat::R32_SFLOAT:
				return GL_R32F;

			case DataFormat::R32_G32_UINT:
				return GL_RG32UI;

			case DataFormat::R32_G32_SINT:
			{
				TUR_LOG_WARN("R32_G32_SINT is not defined in OpenGL. Using GL_RG32UI");
				return GL_RG32UI;
			}

			case DataFormat::R32_G32_SFLOAT:
				return GL_RG32F;

			case DataFormat::R32_G32_B32_UINT:
				return GL_RGB32UI;

			case DataFormat::R32_G32_B32_SINT:
			{
				TUR_LOG_WARN("R32_G32_B32_SINT is not defined in OpenGL. Using GL_RGB32UI");
				return GL_RGB32UI;
			}

			case DataFormat::R32_G32_B32_SFLOAT:
				return GL_RGB32F;

			case DataFormat::R32_G32_B32_A32_UINT:
				return GL_RGBA32UI;

			case DataFormat::R32_G32_B32_A32_SINT:
			{
				TUR_LOG_WARN("R32_G32_B32_A32_SINT is not defined in OpenGL. Using GL_RGBA32UI");
				return GL_RGBA32UI;
			}

			case DataFormat::R32_G32_B32_A32_SFLOAT:
				return GL_RGBA32F;

			case DataFormat::R64_UINT:
			case DataFormat::R64_SINT:
			case DataFormat::R64_SFLOAT:
			case DataFormat::R64_G64_UINT:
			case DataFormat::R64_G64_SINT:
			case DataFormat::R64_G64_SFLOAT:
			case DataFormat::R64_G64_B64_UINT:
			case DataFormat::R64_G64_B64_SINT:
			case DataFormat::R64_G64_B64_SFLOAT:
			case DataFormat::R64_G64_B64_A64_UINT:
			case DataFormat::R64_G64_B64_A64_SINT:
			case DataFormat::R64_G64_B64_A64_SFLOAT:
			{
				TUR_LOG_WARN("64 bit formats are not specified in OpenGL. Using GL_RGBA32UI");
				return GL_RGBA32UI;
			}

			default:
			{
				TUR_LOG_ERROR("Invalid Format. Using default: R32_UINT");
				return GL_R32UI;
			} break;
		}

		return GL_R32UI;
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

	// Buffer:
	inline constexpr uint32_t GetBufferBindingFlag(BindingFlag bindingFlag)
	{
		switch (bindingFlag)
		{
			case BindingFlag::ARRAY_BUFFER:
				return GL_ARRAY_BUFFER;

			case BindingFlag::ATOMIC_COUNTER_BUFFER:
				return GL_ATOMIC_COUNTER_BUFFER;

			case BindingFlag::COPY_READ_BUFFER:
				return GL_COPY_READ_BUFFER;

			case BindingFlag::COPY_WRITE_BUFFER:
				return GL_COPY_WRITE_BUFFER;

			case BindingFlag::DISPATCH_INDIRECT_BUFFER:
				return GL_DISPATCH_INDIRECT_BUFFER;

			case BindingFlag::DRAW_INDIRECT_BUFFER:
				return GL_DRAW_INDIRECT_BUFFER;

			case BindingFlag::ELEMENT_ARRAY_BUFFER:
				return GL_ELEMENT_ARRAY_BUFFER;

			case BindingFlag::PIXEL_PACK_BUFFER:
				return GL_PIXEL_PACK_BUFFER;

			case BindingFlag::PIXEL_UNPACK_BUFFER:
				return GL_PIXEL_UNPACK_BUFFER;

			case BindingFlag::QUERY_BUFFER:
				return GL_QUERY_BUFFER;

			case BindingFlag::SHADER_STORAGE_BUFFER:
				return GL_SHADER_STORAGE_BUFFER;

			case BindingFlag::TEXTURE_BUFFER:
				return GL_TEXTURE_BUFFER;

			case BindingFlag::TRANSFORM_FEEDBACK_BUFFER:
				return GL_TRANSFORM_FEEDBACK_BUFFER;

			case BindingFlag::UNIFORM_BUFFER:
				return GL_UNIFORM_BUFFER;

			default: 
			{
				TUR_LOG_ERROR("Invalid Buffer Binding Flag. Using default: GL_ARRAY_BUFFER");
				return GL_ARRAY_BUFFER;
			} break;
		}
	}

	inline constexpr uint32_t GetInputLayoutType(LayoutType layoutType)
	{
		switch (layoutType)
		{
			case LayoutType::NONE: 
				return GL_NONE;

			case LayoutType::INT_8: 
				return GL_INT;

			case LayoutType::INT_16: 
				return GL_INT;

			case LayoutType::INT_32: 
				return GL_INT;

			case LayoutType::INT_64: 
				return GL_INT;

			case LayoutType::UINT_8: 
				return GL_UNSIGNED_INT;

			case LayoutType::UINT_16: 
				return GL_UNSIGNED_INT;

			case LayoutType::UINT_32: 
				return GL_UNSIGNED_INT;

			case LayoutType::UINT_64: 
				return GL_UNSIGNED_INT;

			case LayoutType::FLOAT_32: 
				return GL_FLOAT;

			case LayoutType::FLOAT_64:
				return GL_DOUBLE;

			default:
			{
				TUR_LOG_ERROR("Invalid Layout Format. Using default: GL_NONE");
				return GL_NONE;
			} break;
		}
	}
}