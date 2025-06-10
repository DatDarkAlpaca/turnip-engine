#include "pch.hpp"
#include "vulkan_device.hpp"
#include "vulkan_deletion_queue.hpp"

namespace tur::vulkan::deletion
{
	void initialize_deletion_queue(DeletionQueue& deletionQueue, GraphicsDeviceVulkan* device)
	{
		deletionQueue.device = device;
	}

	void destroy_general(DeletionQueue& deletionQueue, std::function<void()>&& function)
	{
		deletionQueue.additionalDeletionQueue.push_back(std::move(function));
	}
	void destroy_buffer(DeletionQueue& deletionQueue, buffer_handle handle)
	{
		deletionQueue.bufferHandles.push_back(handle);
	}
	void destroy_texture(DeletionQueue& deletionQueue, texture_handle handle)
	{
		deletionQueue.textureHandles.push_back(handle);
	}
	void destroy_render_target(DeletionQueue& deletionQueue, texture_handle handle)
	{
		deletionQueue.renderTargetHandles.push_back(handle);
	}
	void destroy_descriptor_set(DeletionQueue& deletionQueue, descriptor_handle handle)
	{
		deletionQueue.renderTargetHandles.push_back(handle);
	}

	void flush(DeletionQueue& deletionQueue)
	{
		auto& allocator = deletionQueue.device->get_state().vmaAllocator;
		auto& device = deletionQueue.device->get_state().logicalDevice;
		auto& buffers = deletionQueue.device->get_buffers();
		auto& textures = deletionQueue.device->get_textures();
		auto& renderTargets = deletionQueue.device->get_render_targets();

		// Additional deletion:
		for (const auto& deletionFunction : deletionQueue.additionalDeletionQueue)
			deletionFunction();

		// Buffers:
		for (const auto& handle : deletionQueue.bufferHandles)
		{
			Buffer& buffer = buffers.get(handle);
			vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);

			buffers.remove(handle);
		}

		// Textures:
		for (const auto& handle : deletionQueue.textureHandles)
		{
			Texture& texture = textures.get(handle);

			device.destroyImageView(texture.imageView);
			vmaDestroyImage(allocator, texture.image, texture.allocation);

			textures.remove(handle);
		}

		// Render Targets:
		for (const auto& handle : deletionQueue.renderTargetHandles)
		{
			RenderTarget& renderTarget = renderTargets.get(handle);

			for (const auto& textureHandle : renderTarget.descriptor.colorAttachments)
			{
				Texture& texture = textures.get(textureHandle);

				device.destroyImageView(texture.imageView);
				vmaDestroyImage(allocator, texture.image, texture.allocation);
			}
			
			if (!renderTarget.descriptor.depthAttachment)
			{
				Texture& texture = textures.get(renderTarget.descriptor.depthAttachment);

				device.destroyImageView(texture.imageView);
				vmaDestroyImage(allocator, texture.image, texture.allocation);
			}

			renderTargets.remove(handle);
		}
	}
}

