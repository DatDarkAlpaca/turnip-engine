#pragma once
#include <glad/glad.h>

#include "opengl_common.hpp"
#include "graphics/objects/buffer.hpp"

namespace tur::gl
{
	struct Buffer
	{
		BufferDescriptor descriptor;
		gl_handle handle = invalid_handle;
	};
}

namespace tur::gl
{
	constexpr static inline gl_handle get_buffer_index_type(BufferIndexType type)
	{
		switch (type)
		{
			case BufferIndexType::UNSIGNED_BYTE:
				return GL_UNSIGNED_BYTE;

			case BufferIndexType::UNSIGNED_SHORT:
				return GL_UNSIGNED_SHORT;

			case BufferIndexType::UNSIGNED_INT:
				return GL_UNSIGNED_INT;
		}

		TUR_LOG_ERROR("Invalid Index Buffer Type: {}. Default: GL_UNSIGNED_INT",
			static_cast<int>(type));

		return GL_UNSIGNED_INT;
	}
}

namespace tur::gl
{
	constexpr static inline gl_handle get_buffer_type(BufferType type)
	{
		switch (type)
		{
			case BufferType::VERTEX_BUFFER:
				return GL_ARRAY_BUFFER;

			case BufferType::INDEX_BUFFER:
				return GL_ELEMENT_ARRAY_BUFFER;

			case BufferType::UNIFORM_BUFFER:
				return GL_UNIFORM_BUFFER;
		}

		TUR_LOG_ERROR("Invalid Buffer Type: {}. Default: GL_ARRAY_BUFFER",
			static_cast<int>(type));

		return GL_ARRAY_BUFFER;
	}

	constexpr static inline gl_handle get_buffer_usage(BufferUsage usage)
	{
		// TODO: add copy & other usages

		switch (usage)
		{
			case BufferUsage::STATIC:
				return GL_STATIC_DRAW;

			case BufferUsage::DYNAMIC:
				return GL_DYNAMIC_DRAW;

			case BufferUsage::STREAM:
				return GL_STREAM_DRAW;
		}

		TUR_LOG_ERROR("Invalid Buffer Usage: {}. Default: GL_STATIC_DRAW",
			static_cast<int>(usage));

		return GL_STATIC_DRAW;
	}
}