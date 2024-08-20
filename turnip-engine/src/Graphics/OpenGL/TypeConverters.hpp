#pragma once
#include "CommonGL.hpp"

namespace tur::gl
{
	inline constexpr gl_handle GetCullMode(CullMode cullMode)
	{
		switch (cullMode)
		{
			case CullMode::NONE:
				return GL_NONE;

			case CullMode::FRONT:
				return GL_FRONT;

			case CullMode::BACK:
				return GL_BACK;

			case CullMode::FRONT_AND_BACK:
				return GL_FRONT_AND_BACK;

			default:
			{
				TUR_LOG_ERROR("Invalid Cull Mode. Using default: FRONT");
				return GL_BACK;
			}
		}

		return 0;
	}

	struct DataTypeInfo
	{
		gl_handle format;
		size_t componentSize;
	};

	inline constexpr DataTypeInfo GetDataType(DataType type)
	{
		switch (type)
		{
			case DataType::R32_UINT: 
				return { GL_UNSIGNED_INT, 1 };

			case DataType::R32_SINT: 
				return { GL_INT, 1 };
				
			case DataType::R32_SFLOAT: 
				return { GL_FLOAT, 1 };
				
			case DataType::R64_SFLOAT: 
				return { GL_DOUBLE, 1 };
				
			case DataType::R32G32_SFLOAT: 
				return { GL_FLOAT, 2 };
			
			case DataType::R32G32B32_SFLOAT: 
				return { GL_FLOAT, 3 };
		
			case DataType::R32G32B32A32_SFLOAT: 
				return { GL_FLOAT, 4 };

			default:
			{
				TUR_LOG_ERROR("Invalid Data Type. Using default: R32_UINT");
				return { GL_UNSIGNED_INT, 1 };
			}
		}
	}

	inline constexpr size_t GetDataTypeSingleSize(DataType type)
	{
		switch (type)
		{
			case DataType::R32_UINT:
				return sizeof(unsigned int);

			case DataType::R32_SINT:
				return sizeof(int);

			case DataType::R32_SFLOAT:
				return sizeof(float);

			case DataType::R64_SFLOAT:
				return sizeof(double);

			case DataType::R32G32_SFLOAT:
				return sizeof(float);

			case DataType::R32G32B32_SFLOAT:
				return sizeof(float);

			case DataType::R32G32B32A32_SFLOAT:
				return sizeof(float);

			default:
			{
				TUR_LOG_ERROR("Invalid Data Type. Using default: R32_UINT");
				return sizeof(unsigned int);
			}
		}
	}

	inline constexpr gl_handle GetFrontFace(FrontFace frontFace)
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
				TUR_LOG_WARN("LINES_WITH_ADJACENCY is not supported in OpenGL. Using LINES");
				return GL_LINES;
			}

			case PrimitiveTopology::LINE_STRIPS_WITH_ADJACENCY:
			{
				TUR_LOG_WARN("LINE_STRIPS_WITH_ADJACENCY is not supported in OpenGL. Using LINE_STRIP");
				return GL_LINE_STRIP;
			}

			case PrimitiveTopology::TRIANGLES_WITH_ADJACENCY:
			{
				TUR_LOG_WARN("TRIANGLES_WITH_ADJACENCY is not supported in OpenGL. Using TRIANGLES");
				return GL_TRIANGLES;
			}

			case PrimitiveTopology::TRIANGLE_STRIPS_WITH_ADJACENCY:
			{
				TUR_LOG_WARN("TRIANGLE_STRIPS_WITH_ADJACENCY is not supported in OpenGL. Using TRIANGLE_STRIP");
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
}