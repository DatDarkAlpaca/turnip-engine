#pragma once
#include "Graphics/Base/Buffer.hpp"
#include "CommonGL.hpp"

namespace tur::gl
{
	constexpr inline gl_handle GetBufferUsage(BufferUsage usage)
	{
		switch (usage)
		{
			case BufferUsage::VERTEX_BUFFER:
				return GL_ARRAY_BUFFER;

			case BufferUsage::INDEX_BUFFER:
				return GL_ELEMENT_ARRAY_BUFFER;

			case BufferUsage::UNIFORM_BUFFER:
				return GL_UNIFORM_BUFFER;

			case BufferUsage::SHADER_STORAGE_BUFFER:
				return GL_SHADER_STORAGE_BUFFER;

			case BufferUsage::INDIRECT_BUFFER:
				return GL_DRAW_INDIRECT_BUFFER;

			case BufferUsage::TRANSFER_SRC:
				return GL_COPY_READ_BUFFER;

			case BufferUsage::TRANSFER_DST:
				return GL_COPY_WRITE_BUFFER;

			default:
				TUR_LOG_CRITICAL("OpenGL does not support the following buffer usage: {}", (uint32_t)usage);
		}

		return invalid_handle;
	}

	constexpr inline gl_handle GetBufferMemoryUsage(MemoryUsage memoryUsage)
	{
		switch (memoryUsage) 
		{
			case MemoryUsage::GPU_EXCLUSIVE:
				return GL_STATIC_DRAW;

			case MemoryUsage::CPU_EXCLUSIVE:
				return GL_STATIC_READ;

			case MemoryUsage::CPU_TO_GPU:
				return GL_DYNAMIC_DRAW;

			case MemoryUsage::GPU_TO_CPU:
				return GL_DYNAMIC_READ;

			default:
				TUR_LOG_CRITICAL("OpenGL does not support the following memory buffer usage: {}", (uint32_t)memoryUsage);
		}

		return invalid_handle;
	}
}