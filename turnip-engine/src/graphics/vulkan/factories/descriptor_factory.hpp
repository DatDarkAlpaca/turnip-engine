#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "graphics/objects/descriptor.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	vk::DescriptorPool create_descriptor_pool(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor);

	vk::DescriptorSetLayout create_descriptor_set_layout(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor);

	vk::DescriptorSet create_descriptor_set(vk::Device device, vk::DescriptorPool descriptorPool, std::vector<vk::DescriptorSetLayout> descriptorSetLayout);
}