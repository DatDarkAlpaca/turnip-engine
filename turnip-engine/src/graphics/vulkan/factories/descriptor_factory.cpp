#include "pch.hpp"
#include "descriptor_factory.hpp"
#include "graphics/vulkan/vulkan_device.hpp"
#include "graphics/vulkan/objects/descriptor.hpp"

namespace tur::vulkan
{
	vk::DescriptorPool build_descriptor_pool(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor)
	{
		std::vector<vk::DescriptorPoolSize> poolSizes;

		for (const auto& [_, amount, type, __] : descriptor)
		{
			vk::DescriptorPoolSize poolSize;
			poolSize.type = get_descriptor_type(type);
			poolSize.descriptorCount = amount;
			poolSizes.push_back(poolSize);
		}

		vk::DescriptorPoolCreateInfo poolInfo;
		{
			poolInfo.flags = vk::DescriptorPoolCreateFlags();
			poolInfo.maxSets = 100;
			poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
			poolInfo.pPoolSizes = poolSizes.data();
		}

		try
		{
			return device.createDescriptorPool(poolInfo);
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_CRITICAL("Failed to create descriptor pool");
		}

		return nullptr;
	}

	vk::DescriptorSetLayout build_descriptor_set_layout(vk::Device device, const DescriptorSetLayoutDescriptor& descriptor)
	{
		std::vector<vk::DescriptorSetLayoutBinding> descriptorBindings;
		for (const auto& [binding, amount, type, stages] : descriptor)
		{
			vk::DescriptorSetLayoutBinding descriptorBinding = {};
			descriptorBinding.binding = binding;
			descriptorBinding.descriptorType = get_descriptor_type(type);
			descriptorBinding.stageFlags = get_pipeline_stages(stages);
			descriptorBinding.descriptorCount = amount;

			descriptorBindings.push_back(descriptorBinding);
		}

		vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
		descriptorSetLayoutInfo.bindingCount = static_cast<u32>(descriptorBindings.size());
		descriptorSetLayoutInfo.pBindings = descriptorBindings.data();

		try
		{
			return device.createDescriptorSetLayout(descriptorSetLayoutInfo);
		}
		catch (vk::SystemError& err)
		{
			TUR_LOG_CRITICAL("Failed to create descriptor set layout. {}", err.what());
		}

		return nullptr;
	}
	
	vk::DescriptorSet build_descriptor_set(vk::Device device, vk::DescriptorPool descriptorPool, std::vector<vk::DescriptorSetLayout> descriptorSetLayout)
	{
		vk::DescriptorSetAllocateInfo allocationInfo = {};
		{
			allocationInfo.descriptorPool = descriptorPool;
			allocationInfo.descriptorSetCount = 1;
			allocationInfo.pSetLayouts = descriptorSetLayout.data();
		}

		try
		{
			return device.allocateDescriptorSets(allocationInfo)[0];
		}
		catch (vk::SystemError err)
		{
			TUR_LOG_CRITICAL("Failed to allocate frame descriptor set: {}", err.what());
		}

		return {};
	}
}