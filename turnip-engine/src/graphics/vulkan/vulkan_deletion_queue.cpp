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
		for (auto& handle : deletionQueue.bufferHandles)
		{
			if (handle == invalid_handle)
				continue;

			Buffer& buffer = buffers.get(handle);
			vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);

			buffers.remove(handle);
			handle = invalid_handle;
		}
		deletionQueue.bufferHandles.clear();

		// Textures:
		for (auto& handle : deletionQueue.textureHandles)
		{
			if (handle == invalid_handle)
				continue;

			Texture& texture = textures.get(handle);

			vmaDestroyImage(allocator, texture.image, texture.allocation);
			device.destroyImageView(texture.imageView);
			device.destroySampler(texture.sampler);

			textures.remove(handle);
			handle = invalid_handle;
		}
		deletionQueue.textureHandles.clear();

		// Render Targets:
		for (auto& handle : deletionQueue.renderTargetHandles)
		{
			RenderTarget& renderTarget = renderTargets.get(handle);

			for (auto& textureHandle : renderTarget.descriptor.colorAttachments)
			{
				if (textureHandle == invalid_handle)
					continue;

				if (!textures.constains(textureHandle))
					continue;

				Texture& texture = textures.get(textureHandle);

				vmaDestroyImage(allocator, texture.image, texture.allocation);
				device.destroyImageView(texture.imageView);
				device.destroySampler(texture.sampler);

				textures.remove(handle);
				textureHandle = invalid_handle;
			}
			
			if (!renderTarget.descriptor.depthAttachment)
			{
				Texture& texture = textures.get(renderTarget.descriptor.depthAttachment);

				vmaDestroyImage(allocator, texture.image, texture.allocation);
				device.destroyImageView(texture.imageView);
				device.destroySampler(texture.sampler);

				textures.remove(handle);
				renderTarget.descriptor.depthAttachment = invalid_handle;
			}

			renderTargets.remove(handle);
			handle = invalid_handle;
		}
		deletionQueue.renderTargetHandles.clear();
	}
}

