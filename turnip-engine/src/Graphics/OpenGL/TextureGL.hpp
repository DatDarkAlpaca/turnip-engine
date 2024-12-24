#pragma once
#include "CommonGL.hpp"
#include "Graphics/Base/Texture.hpp"

namespace tur::gl
{
	constexpr inline gl_handle GetTextureFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGBA8: 
				return GL_RGBA8;

			case TextureFormat::RGB8: 
				return GL_RGB8;

			case TextureFormat::BGRA8: 
				return GL_BGRA;

			case TextureFormat::B8G8R8A8_UNORM:
			{
				TUR_LOG_WARN("B8G8R8A8_UNORM is not supported by OpenGL. Using GL_RGBA");
				return GL_RGBA;
			}

			case TextureFormat::D32: 
				return GL_DEPTH_COMPONENT32F;

			case TextureFormat::DEPTH24_STENCIL8:
				return GL_DEPTH24_STENCIL8;

			default: 
				TUR_LOG_CRITICAL("OpenGL does not support the following texture format: {}", (uint32_t)format);
		}

		return invalid_handle;
	}

	constexpr inline gl_handle GetTextureType(TextureType usage)
	{
		switch (usage)
		{
			case TextureType::TEXTURE_2D: 
				return GL_TEXTURE_2D;

			case TextureType::TEXTURE_3D: 
				return GL_TEXTURE_3D;

			case TextureType::CUBE_MAP: 
				return GL_TEXTURE_CUBE_MAP;

			default:
				TUR_LOG_CRITICAL("OpenGL does not support the following texture type: {}", (uint32_t)usage);
		}

		return invalid_handle;
	}

	constexpr inline gl_handle GetWrapMode(WrapMode mode)
	{
		switch (mode)
		{
			case WrapMode::REPEAT: 
				return GL_REPEAT;

			case WrapMode::MIRRORED_REPEAT:
				return GL_MIRRORED_REPEAT;

			case WrapMode::CLAMP_TO_EDGE: 
				return GL_CLAMP_TO_EDGE;

			case WrapMode::CLAMP_TO_BORDER: 
				return GL_CLAMP_TO_BORDER;

			default: 
				TUR_LOG_CRITICAL("OpenGL does not support the following wrap mode: {}", (uint32_t)mode);
		}

		return invalid_handle;
	}

	constexpr inline gl_handle GetFilterMode(FilterMode mode, bool hasMipmaps)
	{
		switch (mode) 
		{
			case FilterMode::NEAREST:
				return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

			case FilterMode::LINEAR:
				return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

			case FilterMode::BILINEAR:
				return GL_LINEAR_MIPMAP_NEAREST;

			case FilterMode::TRILINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;

			default:
				TUR_LOG_CRITICAL("OpenGL does not support the following filter mode: {}", (uint32_t)mode);
		}

		return invalid_handle;
	}
}