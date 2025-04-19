#pragma once
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include "graphics/objects/texture.hpp"

namespace tur::vulkan
{
	struct Texture
	{
		vk::Image image;
		vk::ImageView imageView;
		VmaAllocation allocation = nullptr;
		vk::Extent3D extent;		
		vk::Format format = vk::Format::eR8G8B8A8Unorm;
	};
}

namespace tur::vulkan
{
	constexpr static vk::ImageType get_texture_type(TextureType type)
	{
		switch (type)
		{
			case TextureType::TEXTURE_2D:
				return vk::ImageType::e2D;

			case TextureType::TEXTURE_3D:
				return vk::ImageType::e3D;

			case TextureType::CUBE_MAP:
				return vk::ImageType::e3D;
		}

		TUR_LOG_ERROR("Invalid Texture Type: {}. Default: e2D", static_cast<int>(type));
		return vk::ImageType::e2D;
	}
	constexpr static vk::ImageViewType get_texture_view_type(TextureType type)
	{
		switch (type)
		{
		case TextureType::TEXTURE_2D:
			return vk::ImageViewType::e2D;

		case TextureType::TEXTURE_3D:
			return vk::ImageViewType::e3D;

		case TextureType::CUBE_MAP:
			return vk::ImageViewType::eCube;
		}

		TUR_LOG_ERROR("Invalid Image View Type: {}. Default: e2D", static_cast<int>(type));
		return vk::ImageViewType::e2D;
	}

	// wrapmode
	// filtermode

	constexpr static vk::Format get_texture_format(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::DEPTH_16_UNORM:
				return vk::Format::eD16Unorm;

			case TextureFormat::DEPTH_STENCIL16_S8U_INT:
				return vk::Format::eD16UnormS8Uint;

			case TextureFormat::R8_UNORM:
				return vk::Format::eR8Unorm;

			case TextureFormat::RG8_UNORM:
				return vk::Format::eR8G8Unorm;

			case TextureFormat::RGB8_UNORM:
				return vk::Format::eR8G8B8Unorm;

			case TextureFormat::RGBA8_UNORM:
				return vk::Format::eR8G8B8A8Unorm;

			case TextureFormat::RGBA16_SFLOAT:
				return vk::Format::eR16G16B16A16Sfloat;

			case TextureFormat::B8G8R8A8_UNORM:
				return vk::Format::eB8G8R8A8Srgb;
		}

		TUR_LOG_ERROR("Invalid Texture Format: {}. Default: RGBA (32-bit)", static_cast<int>(format));
		return vk::Format::eR8G8B8A8Srgb;
	}
}