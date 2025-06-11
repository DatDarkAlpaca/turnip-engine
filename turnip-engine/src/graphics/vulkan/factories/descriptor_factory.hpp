#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "graphics/objects/descriptor.hpp"

namespace tur::vulkan
{
	class GraphicsDeviceVulkan;

	vk::DescriptorPool build_descriptor_pool(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor);

	vk::DescriptorSetLayout build_descriptor_set_layout(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor);

	vk::DescriptorSet build_descriptor_set(vk::Device device, vk::DescriptorPool descriptorPool, std::vector<vk::DescriptorSetLayout> descriptorSetLayout);
}