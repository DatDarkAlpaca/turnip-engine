#pragma once
#include <glad/glad.h>
#include <numeric>

#include "Rendering/Resource/Buffer.h"

namespace tur::gl
{
	struct BufferGL
	{
		uint32_t id;
	};

	constexpr inline uint32_t TranslateBindingFlag(BindingFlag flag)
	{
		switch (flag)
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
				TUR_LOG_CRITICAL("Invalid buffer binding flag.");
				break;
		}

		return 0xFFFFFFFF;
	}

	constexpr inline uint32_t TranslateUsageFlag(UsageFlag flag)
	{
		switch (flag)
		{
			case UsageFlag::STREAM_DRAW:
				return GL_STREAM_DRAW;

			case UsageFlag::STREAM_READ:
				return GL_STREAM_READ;

			case UsageFlag::STREAM_COPY:
				return GL_STREAM_COPY;

			case UsageFlag::STATIC_DRAW:
				return GL_STATIC_DRAW;

			case UsageFlag::STATIC_READ:
				return GL_STATIC_READ;

			case UsageFlag::STATIC_COPY:
				return GL_STATIC_COPY;

			case UsageFlag::DYNAMIC_DRAW:
				return GL_DYNAMIC_DRAW;

			case UsageFlag::DYNAMIC_READ:
				return GL_DYNAMIC_READ;

			case UsageFlag::DYNAMIC_COPY:
				return GL_DYNAMIC_COPY;

			default:
				TUR_LOG_CRITICAL("Invalid buffer usage flag.");
				break;
		}

		return 0xFFFFFFFF;
	}
}