#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "common.hpp"
#include "graphics/vulkan/objects/texture.hpp"
#include "graphics/vulkan/objects/buffer.hpp"
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

		NON_OWNING GraphicsDeviceVulkan* device = nullptr;
	};

	void initialize_deletion_queue(DeletionQueue& deletionQueue, GraphicsDeviceVulkan* device);

	void destroy_buffer(DeletionQueue& deletionQueue, buffer_handle handle);
	void destroy_texture(DeletionQueue& deletionQueue, texture_handle handle);
	void destroy_render_target(DeletionQueue& deletionQueue, texture_handle handle);
	
	void flush(DeletionQueue& deletionQueue);
}