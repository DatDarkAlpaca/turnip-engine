#pragma once
#include <glad/glad.h>
#include "graphics/objects/texture.hpp"

namespace tur::gl
{
	struct Texture
	{
		TextureDescriptor descriptor;
		texture_handle handle = invalid_handle;
	};
}

namespace tur::gl
{
	constexpr static gl_handle get_texture_type(TextureType type)
	{
		switch (type)
		{
			case TextureType::TEXTURE_2D:
				return GL_TEXTURE_2D;

			case TextureType::TEXTURE_3D:
				return GL_TEXTURE_3D;

			case TextureType::CUBE_MAP:
				return GL_TEXTURE_CUBE_MAP;
		}

		TUR_LOG_ERROR("Invalid Texture Type: {}. Default: GL_TEXTURE_2D",
			static_cast<int>(type));

		return GL_TEXTURE_2D;
	}

	constexpr static gl_handle get_wrap_mode(WrapMode mode)
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
		}

		TUR_LOG_ERROR("Invalid Texture Wrap Mode: {}. Default: GL_REPEAT",
			static_cast<int>(mode));

		return GL_REPEAT;
	}

	constexpr static gl_handle get_filter_mode(FilterMode mode, bool isMinFilter)
	{
		if (isMinFilter)
		{
			switch (mode)
			{
			case FilterMode::NEAREST:
				return GL_NEAREST;

			case FilterMode::LINEAR:
				return GL_LINEAR;

			case FilterMode::BILINEAR:
				return GL_LINEAR_MIPMAP_NEAREST;

			case FilterMode::TRILINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
			}
		}
		else
		{
			switch (mode)
			{
			case FilterMode::NEAREST:
				return GL_NEAREST;

			case FilterMode::LINEAR:
				return GL_LINEAR;

			case FilterMode::BILINEAR:
				return GL_LINEAR;

			case FilterMode::TRILINEAR:
				return GL_LINEAR;
			}
		}

		TUR_LOG_ERROR("Invalid Texture Wrap Mode: {}. Default: GL_NEAREST",
			static_cast<int>(mode));

		return GL_REPEAT;
	}

	constexpr static gl_handle get_texture_format(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::DEPTH_COMPONENT:
				return GL_DEPTH_COMPONENT;

			case TextureFormat::DEPTH_STENCIL:
				return GL_DEPTH_STENCIL;

			case TextureFormat::RED:
				return GL_RED;

			case TextureFormat::RG:
				return GL_RG;

			case TextureFormat::RGB:
				return GL_RGB;

			case TextureFormat::RGBA:
				return GL_RGBA;
		}

		TUR_LOG_ERROR("Invalid Texture Format: {}. Default: GL_RGBA",
			static_cast<int>(format));

		return GL_RGBA;
	}
}