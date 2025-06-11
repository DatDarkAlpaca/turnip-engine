#pragma once
#include <vector>
#include <functional>
#include <vulkan/vulkan.hpp>

#include "common.hpp"
#include "graphics/vulkan/objects/texture.hpp"
#include "graphics/vulkan/objects/buffer.hpp"
#include "graphics/vulkan/objects/descriptor.hpp"
#include "core/free_list.hpp"

namespace tur::vulkan 
{
	class GraphicsDeviceVulkan;
}

namespace tur::vulkan::deletion
{
	struct DeletionQueue
	{
		std::vector<buffer_handle> bufferHandles;
		std::vector<texture_handle> textureHandles;
		std::vector<texture_handle> renderTargetHandles;
		std::vector<std::function<void()>> additionalDeletionQueue;

		NON_OWNING GraphicsDeviceVulkan* device = nullptr;
	};

	void initialize_deletion_queue(DeletionQueue& deletionQueue, GraphicsDeviceVulkan* device);

	void destroy_general(DeletionQueue& deletionQueue, std::function<void()>&& function);
	void destroy_buffer(DeletionQueue& deletionQueue, buffer_handle textureHandle);
	void destroy_texture(DeletionQueue& deletionQueue, texture_handle textureHandle);
	void destroy_render_target(DeletionQueue& deletionQueue, texture_handle textureHandle);
	void destroy_descriptor_set(DeletionQueue& deletionQueue, descriptor_handle textureHandle);
	
	void flush(DeletionQueue& deletionQueue);
}