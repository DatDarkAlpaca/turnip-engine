#include "pch.hpp"
#include "texture_factory.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/factories/buffer_factory.hpp"

namespace tur::vulkan
{
	Texture build_texture(GraphicsDeviceVulkan* device, const TextureDescriptor& descriptor, const TextureAsset& asset)
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
				usageFlags |= vk::ImageUsageFlagBits::eSampled;
			}
			imageCreateInfo.usage = usageFlags;
		}

		VmaAllocationCreateInfo imageAllocationInfo = {};
		{
			imageAllocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			imageAllocationInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		}

		if (vmaCreateImage(
			device->get_state().vmaAllocator,
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

		// Data:
		if (asset.data.size > 0 && asset.data.data)
		{
			BufferDescriptor stagingDescriptor = {};
			{
				stagingDescriptor.memoryUsage = BufferMemoryUsage::CPU_ONLY;
				stagingDescriptor.type = BufferType::TRANSFER_SRC | BufferType::TRANSFER_DST;
			}

			buffer_handle stagingBufferHandle = device->build_buffer(stagingDescriptor, asset.data.size);
			device->update_buffer(stagingBufferHandle, asset.data, 0);
			Buffer& stagingBuffer = device->get_buffers().get(stagingBufferHandle);

			{
				device->transition_texture_layout(texture, texture.layout, vk::ImageLayout::eTransferDstOptimal);

				device->copy_buffer_to_texture_direct(stagingBuffer, texture, descriptor.width, descriptor.height);

				device->transition_texture_layout(texture, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
			}
			device->destroy_buffer(stagingBufferHandle);
		}
		else
			device->transition_texture_layout(texture, vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal);

		texture.layout = vk::ImageLayout::eShaderReadOnlyOptimal;

		// Image view:
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

		texture.imageView = device->get_state().logicalDevice.createImageView(imageViewCreateInfo);

		// Sampler:
		vk::SamplerCreateInfo samplerInfo = {};
		{
			samplerInfo.minFilter = get_filter_mode(descriptor.minFilter, true);
			samplerInfo.magFilter = get_filter_mode(descriptor.magFilter, false);
			samplerInfo.mipmapMode = get_mipmap_mode(descriptor.minFilter);

			samplerInfo.addressModeU = get_wrap_mode(descriptor.wrapR);
			samplerInfo.addressModeV = get_wrap_mode(descriptor.wrapS);
			samplerInfo.addressModeW = get_wrap_mode(descriptor.wrapT);

			// TODO: fill in the rest of the sampler options
			samplerInfo.anisotropyEnable;
			samplerInfo.maxAnisotropy;

			samplerInfo.compareEnable;
			samplerInfo.compareOp;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.mipLodBias = 0.0f;

			samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
			samplerInfo.unnormalizedCoordinates = false;
		}

		texture.sampler = device->get_state().logicalDevice.createSampler(samplerInfo);
		texture.descriptor = descriptor;

		return texture;
	}
}