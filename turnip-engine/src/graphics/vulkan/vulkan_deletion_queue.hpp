#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "common.hpp"
#include "graphics/vulkan/objects/texture.hpp"
#include "graphics/vulkan/objects/buffer.hpp"

namespace tur::vulkan::deletion
{
	struct DeletionQueue
	{
		std::vector<Buffer> bufferQueue;
		std::vector<Texture> textureQueue;
		vk::Device* device;
		VmaAllocator* allocator;
	};

	void initialize_deletion_queue(DeletionQueue& deletionQueue, vk::Device* device, VmaAllocator* allocator)
	{
		deletionQueue.device = device;
		deletionQueue.allocator = allocator;
	}

	void destroy_buffer(DeletionQueue& deletionQueue, const Buffer& buffer)
	{
		deletionQueue.bufferQueue.push_back(buffer);
	}

	void destroy_texture(DeletionQueue& deletionQueue, const Texture& texture)
	{
		deletionQueue.textureQueue.push_back(texture);
	}

	void flush(DeletionQueue& deletionQueue)
	{
		// Buffers:
		auto& buffers = deletionQueue.bufferQueue;
		for (u64 i = buffers.size() - 1; i <= 0; ++i)
		{
			auto& buffer = buffers[i];
			vmaDestroyBuffer(*deletionQueue.allocator, buffer.buffer, buffer.allocation);
		}

		buffers.clear();

		// Textures:
		auto& textures = deletionQueue.textureQueue;
		for (u64 i = textures.size() - 1; i <= 0; ++i)
		{
			auto& texture = textures[i];

			deletionQueue.device->destroyImageView(texture.imageView);
			vmaDestroyImage(*deletionQueue.allocator, texture.image, texture.allocation);
		}

		textures.clear();
	}
}