#pragma once
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include "graphics/vulkan/objects/texture.hpp"

namespace tur::vulkan
{
	inline Texture create_texture(VmaAllocator allocator, vk::Device device, const TextureDescriptor& descriptor)
	{
		Texture texture;
		texture.format = get_texture_format(descriptor.format);
		texture.extent = vk::Extent3D{ descriptor.width, descriptor.height, 1 };

		vk::ImageCreateInfo imageCreateInfo = {};
		{
			imageCreateInfo.flags = vk::ImageCreateFlags();
			imageCreateInfo.imageType = get_texture_type(descriptor.type);

			imageCreateInfo.format = texture.format;
			imageCreateInfo.extent = texture.extent;

			imageCreateInfo.mipLevels = descriptor.mipLevels;
			imageCreateInfo.arrayLayers = 1;

			// TODO: support multisampling.
			imageCreateInfo.samples = vk::SampleCountFlagBits::e1;

			// TODO: support linear tiling for CPU data.
			imageCreateInfo.tiling = vk::ImageTiling::eOptimal;

			// TODO: descriptor for usage flags.
			vk::ImageUsageFlags usageFlags;
			{
				usageFlags |= vk::ImageUsageFlagBits::eTransferSrc;
				usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
				usageFlags |= vk::ImageUsageFlagBits::eStorage;
				usageFlags |= vk::ImageUsageFlagBits::eColorAttachment;
			}
			imageCreateInfo.usage = usageFlags;
		}

		VmaAllocationCreateInfo imageAllocationInfo = {};
		{
			imageAllocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			imageAllocationInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		}

		if (vmaCreateImage(
			allocator,
			reinterpret_cast<const VkImageCreateInfo*>(&imageCreateInfo),
			&imageAllocationInfo,
			reinterpret_cast<VkImage*>(&texture.image),
			&texture.allocation,
			nullptr
		) != VK_SUCCESS)
		{
			TUR_LOG_ERROR("Failed to create image");
			return {};
		}

		vk::ImageViewCreateInfo imageViewCreateInfo = {};
		{
			imageViewCreateInfo.flags = vk::ImageViewCreateFlags();
			imageViewCreateInfo.viewType = get_texture_view_type(descriptor.type);
			imageViewCreateInfo.image = texture.image;
			imageViewCreateInfo.format = texture.format;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			// TODO: descriptor aspect flags
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		}

		texture.imageView = device.createImageView(imageViewCreateInfo);
		return texture;
	}
}