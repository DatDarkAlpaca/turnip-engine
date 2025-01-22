#pragma once
#include <vulkan/vulkan.hpp>
#include "graphics/objects/texture.hpp"

namespace tur::vulkan
{
	constexpr static vk::Format get_texture_format(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::DEPTH_COMPONENT:
				return vk::Format::eD16Unorm;

			case TextureFormat::DEPTH_STENCIL:
				return vk::Format::eD16UnormS8Uint;

			case TextureFormat::RED:
				return vk::Format::eR8Unorm;

			case TextureFormat::RG:
				return vk::Format::eR8G8Unorm;

			case TextureFormat::RGB:
				return vk::Format::eR8G8B8Unorm;

			case TextureFormat::RGBA:
				return vk::Format::eR8G8B8A8Unorm;

			case TextureFormat::B8G8R8A8_UNORM:
				return vk::Format::eB8G8R8A8Unorm;
		}

		TUR_LOG_ERROR("Invalid Texture Format: {}. Default: eR8G8B8A8Unorm",
			static_cast<int>(format));

		return vk::Format::eR8G8B8A8Unorm;
	}
}